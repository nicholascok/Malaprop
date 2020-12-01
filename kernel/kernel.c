#include "../include/kernel.h"

	
extern void kmain(void) {
	const char* str1 = "HELLOWORLD\0";
	char colour1 = 0x1F;
	print_string(str1, colour1);
}

volatile void print_string (const char* str, char colour) {
	volatile char* TEXT_BUFFER = (volatile char*)0xB8000;
	while (*str != 0) {
		*TEXT_BUFFER++ = *str++;
		*TEXT_BUFFER++ = colour;
	}
}
