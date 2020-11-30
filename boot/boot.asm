[BITS 16]
[ORG 0x7C00]

JMP 0x0000:_start

global _start
_start:
; BiOS loads boot drive into the dl register:
; save this value for later use.
MOV [boot_drive], dl

; Initialize registers

; enable the A20 line
MOV ax, 0x2401
INT 0x15

; enable video mode 03h (80 x 25 Text mode)
MOV ah, 0x00; int 10h, func 00h
MOV al, 0x03; video mode (3)
INT 0x10

; print initialization message
MOV si, initialization_message
CALL print

; reset disk
MOV ah, 0x00; int 13h, func 00h
MOV dl, [boot_drive]; drive number (boot drive, given by BiOS)
INT 0x13

; carry flag is set if there was an error:
MOV si, error_message
JC print_error

; load the kernel into memory at 0x100000 (1 MiB)
MOV bx, 0x1000
MOV es, bx
MOV bx, 0x0000
MOV ah, 0x02; int 13h, func 02h
MOV ch, 0x00; cylinder number (0)
MOV dh, 0x00; head number (0)
MOV cl, 0x02; first sector to read (2)
MOV al, 0x10; number of sectors to read (16)
MOV dl, [boot_drive]; drive number (boot drive, given by BiOS)
INT 0x13

; carry flag is set if there was an error:
; (only try to read the drive once because
; im lazy and don't care)
MOV si, error_message
JC print_error

; disable interrupts
CLI

; load the gdt
LGDT [gdtr]

; enter protected mode
MOV eax, cr0
OR eax, 0x1
MOV cr0, eax

; reload segment registers (excluding the cs register) by initializing
; them with a reference to the new data selector.
; 0x10 points to new data selector (16-bit offset from start of gdt)
MOV ax, 0x10
MOV ds, ax
MOV es, ax
MOV fs, ax
MOV gs, ax
MOV ss, ax

; far jump to kernel (also reload the cs (code segment) register, since
; it cannot be diretly accessed).
; 0x08 points to new code selector (8-bit offset from start of gdt)
JMP 0x08:0x10000



; setup gdt (global descriptor table)

; note: the segment base is the address of the start of that segment
; and the offset limit denotes the maximum offset that can be used
; to access data from the segment, think of it as the size of the segment.

gdt_num_entries EQU 0x03
gdt_entry_size EQU 0x08

ALIGN 8
DB 0x00

gdt:
; Descriptor 0: null (empty)
	DQ 0x00000000
; Descriptor 1: kernel code segment
; base = 0x00000000, limit = 0xFFFFF
.code:
	DW 0xFFFF; offset limit bytes 0-1
	DW 0x0000; segment base address bytes 0-1
	DB 0x00; segment base address byte 2
	DB 0x9A; access byte (10011010b)
	DB 0xCF; flags (1100b) + last 4 bits of offset limit (F)
	DB 0x00; segment base address byte 3
; Descriptor 2: kernel data and stack segment
; base = 0x00000000, limit = 0xFFFFF
.data:
	DW 0xFFFF; offset limit bytes 0-1
	DW 0x0000; segment base address bytes 0-1
	DB 0x00; segment base address byte 2
	DB 0x92; access byte (10010010b)
	DB 0xCF; flags (1100b) + last 4 bits of offset limit (1111b)
	DB 0x00; segment base address byte 3

; define a 48-bit sequence used to load the gdt:
gdtr:
	DW gdt_num_entries * gdt_entry_size; 16-bits - size of gdt (24)
	DD gdt; 32-bits - location of gdt in memory

; move cursor to next line
new_line:
	XOR bh, bh;
	MOV ah, 0x03; int 10h, func 03h
	INT 0x10
	XOR dl, dl; cursor column (0)
	INC dh; increment cursor row
	MOV ah, 0x02; int 10h, func 02h
	INT 0x10
	RET

; print message(from si register) to the screen
print:
	XOR bh, bh; page number (0)
	MOV ah, 0x0E; int 10h, func 0Eh
	MOV bl, 0x0F; text colour (white)

next_char:
	MOV al, [si]; character to print
	OR al, al
	JZ exit
	INT 0x10
	INC si
	JMP next_char

exit:
	RET

print_error:
	CALL new_line
	CALL print
	; Retrieve error details
	MOV ah, 0x01; int 13h, func 01h
	MOV dl, [boot_drive]; drive number (boot drive, given by BiOS)
	INT 0x13
	; error code is stored in ah:
	; we add 0x41 ("A") so that all the error codes align with characters that can be printed
	ADD ah, 0x41
	; print error code to the screen
	MOV [error_code], ah
	MOV si, error_code
	CALL print
	RET

boot_drive:
	DB 0x00

error_code:
	DW 0x0000

kjump:
	BITS 32
	JMP 0x08:0x10000

initialization_message DB "Loading Kernel...", 0
error_message DB "DISK ERROR: CODE ", 0

; fill the rest of the boot sector with padded zeros
TIMES 510-($-$$) DB 0
DW 0xAA55; boot signature

; Start of sector 2 (this code is copied into memory)

[BITS 32]

MOV bx, 0xB800
MOV es, bx
MOV bx, 0x0000
MOV DWORD[es:bx], "TEST"

JMP $
