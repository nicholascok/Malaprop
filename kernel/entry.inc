%ifndef __ENTRY_INC__
%define __ENTRY_INC__

[BITS 32]

MOV esp, stack_top

; test
MOV DWORD[0xB8000], "T?E?"
MOV DWORD[0xB8004], "S?T?"

JMP $

stack_bottom: EQU $
	RESB 16384; reserve 16KiB for the stack
stack_top:

%endif
