ENTRY(_boot)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x7C00;
	.text : { *(.boot) }
	. = 0x10000;
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
