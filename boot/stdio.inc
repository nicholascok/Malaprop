%ifndef __STDIO_INC__
%define __STDIO_INC__

; clear the screen and move cursor to top-left corner of screen
clear:
	; clear the screen
	MOV bh, 0x0F; blank-line colour (black)
	MOV cl, 0x00; column of upper-left corner (0)
	MOV ch, 0x00; row of upper-left corner (0)
	MOV dl, 0x50; column of bottom-right corner (80)
	MOV dh, 0x19; row of bottom-right corner (25)
	MOV al, 0x00; number of lines to scroll (0 for entire window)
	MOV ah, 0x06; int 10h, func 06h
	INT 0x10
	
	; originate cursor
	XOR bh, bh; page number (0)
	XOR dl, dl; cursor column (0)
	XOR dh, dh; cursor row (0)
	MOV ah, 0x02; int 10h, func 02h
	INT 0x10
	RET

; move cursor to next line
new_line:
	; get cursor position
	XOR bh, bh; page number (0)
	MOV ah, 0x03; int 10h, func 03h
	INT 0x10
	; move cursor to next line
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

%endif
