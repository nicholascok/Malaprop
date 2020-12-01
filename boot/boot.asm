[BITS 16]
;;;;[ORG 0x7C00]

JMP 0x0000:_boot

%include "boot/stdio.inc"
%include "boot/gdt.inc"

error:
	MOV si, error_message
	CALL new_line
	CALL print
	; Retrieve error details
	MOV ah, 0x01; int 13h, func 01h
	MOV dl, [boot_drive]; drive number (boot drive, given by BiOS)
	INT 0x13
	; error code is stored in ah:
	; we add 0x41 ("A") so that all error codes align with characters that can be printed
	ADD ah, 0x41
	; print error code to the screen
	MOV [error_code], ah
	MOV si, error_code
	CALL print
	CLI
	JMP $

initialization_message DB "Loading Kernel...", 0
error_message DB "DISK ERROR: CODE ", 0
return_message DB "You have returned to the boot sector...", 0

error_code:
	DW 0x0000

boot_drive:
	DB 0x00

global _boot
_boot:
; BiOS loads boot drive into the dl register:
; save this value for later use.
MOV [boot_drive], dl

; initialize registers
XOR ax, ax
MOV ds, ax
MOV es, ax

; setup temporary stack: 0x0000 to 0x7C00
MOV ss, ax
MOV sp, 0x7C00

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
JC error

; load the kernel into memory at 0x10000
MOV bx, 0x1000
MOV es, bx
MOV bx, 0x0000
MOV ah, 0x02; int 13h, func 02h
MOV ch, 0x00; cylinder number (0)
MOV dh, 0x00; head number (0)
MOV cl, 0x02; first sector to read (2)
MOV al, 0x80; number of sectors to read (128)
MOV dl, [boot_drive]; drive number (boot drive, given by BiOS)
INT 0x13

; carry flag is set if there was an error:
; (only try to read the drive once because
; im lazy and don't care)
JC error

CALL load_gdt

CLI; disable interrupts

; enter protected mode
MOV eax, cr0
OR eax, 1
MOV cr0, eax

JMP 0x08:complete_load

[BITS 32]

complete_load:
; setup segment registers
MOV ax, 0x10
MOV ds, ax
MOV es, ax
MOV fs, ax
MOV gs, ax
MOV ss, ax
JMP 0x08:0x10000

CLI
JMP $

; fill the rest of the boot sector with padded zeros
TIMES 510-($-$$) DB 0
DW 0xAA55; boot signature

;;;;%include "kernel/entry.inc"
