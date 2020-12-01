ENTRY(_boot)
OUTPUT_FORMAT(binary)

SECTIONS
{
	. = 0x7C00;
	.text : { *(.boot) }
	. = 64K;
	.text : { *(.text) }
	.rodata : { *(.rodata) }
	.data : { *(.data) }
	.bss : { *(.bss) }
}
