#include "../include/kernel.h"

extern void kmain(void) {
	const char* str1 = "HELLO WORLD";
	char colour1 = 0x1F;
	VGA_PRINT(str1, colour1);
}
