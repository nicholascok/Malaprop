%ifndef __ENTRY_INC__
%define __ENTRY_INC__

[BITS 32]

SECTION .text
global _start
_start:

MOV esp, stack_top
MOV ebp, esp

EXTERN kmain
CALL kmain

JMP $

SECTION .bss
ALIGN 32
stack_bottom: EQU $
	RESB 16384; reserve 16KiB for the stack
stack_top:

%endif
