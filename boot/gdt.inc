%ifndef __GDT_INC__
%define __GDT_INC_

load_gdt:
	CLI		; disable interrupts
	PUSHA		; save registers onto stack
	LGDT [gdt_dsc]	; load the the gdt into the gdtr register
	POPA		; restore regiters
	STI		; re-enable interrupts
	RET

; setup gdt (global descriptor table)

; note: the segment base is the address of the start of that segment
; and the limit denotes the maximum offset that can be used
; to access data from the segment, think of it as the size of the segment.

gdt_num_entries EQU 0x03
gdt_entry_size EQU 0x08

ALIGN 8
DB 0x00

gdt:
; Descriptor 0: null (empty)
	DQ 0
; Descriptor 1: kernel code segment
; base = 0x00000000, limit = 0xFFFFF
.code:
	DW 0xFFFF	; limit (bits 0-15)
	DW 0x0000	; base (bits 0-15)
	DB 0x00	; base (bits 16-23)
	DB 0x9A	; access byte (10011010b)
	DB 0xCF	; flags (1100b) + limit (bits 16-19)
	DB 0x00	; base (bits 24-31)
; Descriptor 2: kernel data and stack segment
; base = 0x00000000, limit = 0xFFFFF
.data:
	DW 0xFFFF	; limit (bits 0-15)
	DW 0x0000	; base (bits 0-15)
	DB 0x00	; base (bits 16-23)
	DB 0x92	; access byte (10010010b)
	DB 0xCF	; flags (1100b) + limit (bits 16-19)
	DB 0x00	; base (bits 24-31)

; define a 48-bit sequence used to load the gdt:
gdt_dsc:
	DW gdt_num_entries * gdt_entry_size	; 16-bits - size of gdt (24)
	DD gdt					; 32-bits - location of gdt in memory

%endif
