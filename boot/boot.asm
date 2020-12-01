[BITS 16]
;[ORG 0x7C00]

JMP 0x0000:_start

%include "./vga16.inc"
%include "./gdt.inc"

find_msg DB "Locating kernel...", 0
load_msg DB "Loading kernel...", 0
find_err_msg DB "ERROR: failed to locate kernel.", 0
load_err_msg DB "DISK ERROR: CODE ", 0

global _start
_start:
	;setup segment registers
	XOR ax, ax
	MOV ds, ax
	MOV es, ax
	MOV fs, ax
	MOV gs, ax
	
	; setup stack (0x0000 - 0x7C00)
	MOV ss, ax
	MOV sp, 0x7BFF
	
	
