#include "../../include/drivers/mlprp_vga.h"

void VGA_PRINT(const char* str, uint8_t colour) {
	volatile char* TEXT_BUFFER = (volatile char*) 0xB8000;
	while (*str != 0) {
		*TEXT_BUFFER++ = *str++;
		*TEXT_BUFFER++ = colour;
	}
}
