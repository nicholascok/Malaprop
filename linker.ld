ENTRY(_start)
OUTPUT_FORMAT(binary)

STARTUP(boot)
INCLUDE
(
	mlprp_cdef.h
	kernel.c
)

SECTIONS
{
	. = 0x7E00;
	.text : { *(.text) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
