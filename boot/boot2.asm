[BITS 16]
[ORG 0x7C00]

_start:

MOV [boot_drive], dl

MOV ah, 0x00
MOV al, 0x03
INT 0x10

MOV ah, 0x02
MOV al, 16
MOV ch, 0
MOV cl, 2
MOV dh, 0
MOV dl, [boot_drive]
MOV bx, 0x7E00
INT 0x13

JMP 0x7E00

boot_drive:
	DB 0x00

TIMES 510-($-$$) DB 0x00
DW 0xAA55; boot signature

MOV bx, 0xB800
MOV es, bx
MOV bx, 0x0000
MOV DWORD[es:bx], "TEST"
