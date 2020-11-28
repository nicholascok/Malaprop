[BITS 16]
[ORG 0x7C00]

; enable video mode 02h (80 x 25 Text mode)
MOV ah, 0x00; int 10h, func 00h
MOV al, 0x02; video mode (2)
INT 0x10

JMP _start

;move cursor to next line
new_line:
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

print_char:
MOV al, [si]; character to print
OR al, al
JZ exit
INT 0x10
INC si
JMP print_char

exit:
	RET

_start:

; print initialization message
MOV si, initialization_message
CALL print

; load the kernel into memory at 0x10000
MOV ah, 0x02; int 13h, func 02h
MOV al, 0x40; number of sectors to read (64)
MOV ch, 0x00; cylinder number (0)
MOV cl, 0x02; starting sector number (2)
MOV dh, 0x00; head number (0)
MOV dl, 0x00; drive number (floppy disk 0)
; (0x00-0x7F are for floppy disks and 0x80-0xFF are for fixed drives)

; copy data into physical address 0x10000
MOV dx, 0x1000
MOV es, dx
MOV bx, 0x0000
INT 0x13

; carry flag is set if there was an error
JC success

; print error
MOV si, read_error
CALL new_line
CALL print

success:

CLI; disable interrupts

; setup gdt (global descriptor table)

; note: the segment base is the address of the start of that segment
; and the offset limit denotes the maximum offset that can be used
; to access data from the segment, you can think of it as the size
; of the segment.

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
	DB 0xCF; flags (1100b) + last 4 bits of offset limit (1111b)
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

; load the gdt
load_gdt:
LGDT [gdtr]
; far jump to reload the cs (code segment) register, since it cannot
; be diretly accessed.
; 0x08 points to new code selector (8-bit offset from start of gdt)
JMP 0x08:complete_load

; reload segment registers by initializing them with a reference to the
; new data selector.
; 0x10 points to new data selector (16-bit offset from start of gdt)
complete_load:
MOV ax, 0x10
MOV ds, ax
MOV es, ax
MOV fs, ax
MOV gs, ax
MOV ss, ax
RET

; enter protected mode
MOV eax, cr0
OR eax, 1
MOV cr0, eax

; far jump to kernel
;JMP 0x08:0x10000
JMP 0x1000:0x0000

initialization_message DB "Initalizing Kernel...", 0
read_error DB "READ ERROR: Could not retreive kernel from disk.", 0

; fill the rest of the boot sector with padded zeros
TIMES 510-($-$$) DB 0
DW 0xAA55; boot signature

[BITS 32]

; setup stack

MOV esp, stack_bottom
MOV DWORD[0xB0000], 0x07690748

ALIGN 32
stack_bottom: EQU $
RESB 16384; reserve 16KiB of memory
stack_top:
