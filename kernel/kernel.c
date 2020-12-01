#include "../include/kernel.h"

	
extern void kmain(void) {
	const char str1[] = "HELLOWORLD\0";
	char colour1 = 0x1F;
	VGA_PRINT(str1, colour1);
}
