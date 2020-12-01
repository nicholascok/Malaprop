%ifndef __STDIO_INC__
%define __STDIO_INC__

set_mode:

; clear screen and reset cursor position
clear:
	MOV bh, 0x0F	; blank-line colour (black)
	MOV cl, 0x00	; column of upper-left corner (0)
	MOV ch, 0x00	; row of upper-left corner (0)
	MOV dl, 0x50	; column of bottom-right corner (80)
	MOV dh, 0x19	; row of bottom-right corner (25)
	MOV al, 0x00	; number of lines to scroll (0 for entire window)
	MOV ah, 0x06	; int 10h, func 06h
	INT 0x10
	CALL reset_cursor
	RET

; get cursor position
; RET : CH (cursor start line), CL (cursor end line), DH (row), DL (column)
get_cursor:
	XOR bh, bh	; page number (0)
	MOV ah, 0x03	; int 10h, func 03h
	INT 0x10

; reset cursor position
reset_cursor:
	XOR bh, bh	; page number (0)
	XOR dh, dh	; row (0)
	XOR dl, dl	; column (0)
	MOV ah, 0x02	; int 10h, func 03h
	INT 0x10

; move cursor to next line
new_line:
	CALL get_cursor
	XOR dl, dl	; cursor column (0)
	INC dh		; increment cursor row
	MOV ah, 0x02	; int 10h, func 02h
	INT 0x10
	RET

; print message(from si register) at cursor position
print:
	XOR bh, bh	; page number (0)
	MOV ah, 0x0E	; int 10h, func 0Eh
	MOV bl, 0x0F	; text colour (white)

next_char:
	MOV al, [si]	; character to print
	OR al, al	; string ends with nul char:
	JZ exit	; if we are at nul char, exit
	INT 0x10
	INC si
	JMP next_char

exit:
	RET

%endif
