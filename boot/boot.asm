[BITS 16]
[ORG 0x7C00]

; enable video mode 02h (80 x 25 Text mode)
MOV ah, 0x00
MOV al, 0x02
INT 0x10

; message to display
msg DB "Loading keRnel..."
len EQU 17

; print message to the screen
XOR bh, bh
MOV cx, len
MOV bp, msg
MOV dh, 0x00
MOV dl, 0x00
MOV bl, 0x0F
MOV al, 0x01
MOV ah, 0x13
INT 0x10

; load the kernel into memory at 0x10000

JMP $
;JMP 0x7E00

TIMES 510-($-$$) DB 0
DW 0xAA55

; reserve memory for the stack starting at 0x7E00 and ending at 0xBE00
;stack_bottom:
;resb 16384; 16KiB
;stack_top:
