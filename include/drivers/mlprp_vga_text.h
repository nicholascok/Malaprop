#ifndef __MLPRP_VGA_TEXT__
#define __MLPRP_VGA_TEXT__

#include "../../libc/mlprp_cdef.h"

volatile char* VGA_TEXT_BUFFER;

uint8_t VGA_COLOUR;
bool CURSOR_ENABLED;

uint8_t VGA_WIDTH;
uint8_t VGA_HEIGHT;
uint8_t VGA_BYTES_PER_CHAR;
struct { uint8_t x; uint8_t y; } CURSOR;

void VGA_SET_CURSOR(uint8_t _x, uint8_t _y);
void VGA_UPDATE_CURSOR();
void VGA_CURSOR_ENABLE(uint8_t cursor_start, uint8_t cursor_end);
void VGA_CURSOR_DISABLE();

void VGA_CLEAR();
void VGA_NEWLINE();
void VGA_PRINT(const char* str, uint8_t colour);

uint16_t VGA_GET_OFFSET(uint8_t _x, uint8_t _y);
uint8_t VGA_GET_X (uint16_t offset);
uint8_t VGA_GET_Y (uint16_t offset);

#endif
