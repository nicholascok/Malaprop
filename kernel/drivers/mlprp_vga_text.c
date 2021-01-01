#include "../../include/drivers/mlprp_vga_text.h"

volatile char* VGA_TEXT_BUFFER = (volatile char*) 0xB8000;

uint8_t VGA_COLOUR = 0x0F;
bool CURSOR_ENABLED = 1;

uint8_t VGA_WIDTH = 80;
uint8_t VGA_HEIGHT = 25;
uint8_t VGA_BYTES_PER_CHAR = 2;

void VGA_SET_CURSOR(uint8_t _x, uint8_t _y) {
	CURSOR.x = _x;
	CURSOR.y = _y;
	VGA_UPDATE_CURSOR();
}

void VGA_UPDATE_CURSOR() {
	if (CURSOR_ENABLED) {
		uint16_t offset = CURSOR.y * VGA_WIDTH + CURSOR.x;
		outb(0x3D4, 0x0F);
		outb(0x3D5, (uint8_t) (offset & 0xFF));
		outb(0x3D4, 0x0E);
		outb(0x3D5, (uint8_t) ((offset >> 8) & 0xFF));
	}
}

void VGA_CURSOR_ENABLE(uint8_t cursor_start, uint8_t cursor_end) {
	outb(0x3D4, 0x0A);
	outb(0x3D5, (inb(0x3D5) & 0xC0) | cursor_start);
	outb(0x3D4, 0x0B);
	outb(0x3D5, (inb(0x3D5) & 0xE0) | cursor_end);
}

void VGA_CURSOR_DISABLE() {
	outb(0x3D4, 0x0A);
	outb(0x3D5, 0x20);
}

void VGA_NEWLINE() {
	CURSOR.x = 0;
	CURSOR.y++;
}

void VGA_PRINT(const char* str, uint8_t colour) {
	volatile char* TEXT_BUFFER = VGA_TEXT_BUFFER + VGA_GET_OFFSET(CURSOR.x, CURSOR.y);
	while (*str != 0) {
		*str == 0x0A && (TEXT_BUFFER += VGA_WIDTH * VGA_BYTES_PER_CHAR - (TEXT_BUFFER - VGA_TEXT_BUFFER) % (VGA_WIDTH * VGA_BYTES_PER_CHAR), str++);
		*TEXT_BUFFER++ = *str++;
		*TEXT_BUFFER++ = colour;
	}
	CURSOR.x = VGA_GET_X((TEXT_BUFFER - VGA_TEXT_BUFFER) / VGA_BYTES_PER_CHAR);
	CURSOR.y = VGA_GET_Y((TEXT_BUFFER - VGA_TEXT_BUFFER) / VGA_BYTES_PER_CHAR);
	VGA_UPDATE_CURSOR();
}

void VGA_CLEAR() {
	for (int i = 0; i < VGA_WIDTH * VGA_HEIGHT; i++) {
		*(VGA_TEXT_BUFFER + 2 * i) = 0x00;
		*(VGA_TEXT_BUFFER + 2 * i + 1) = VGA_COLOUR;
	}
	VGA_SET_CURSOR(0, 0);
}

uint16_t VGA_GET_OFFSET(uint8_t _x, uint8_t _y) {
	return VGA_WIDTH * _y + _x * 2;
}

uint8_t VGA_GET_X(uint16_t offset) {
	return offset % VGA_WIDTH;
}

uint8_t VGA_GET_Y(uint16_t offset) {
	return (offset - offset % VGA_WIDTH) / VGA_WIDTH;
}
