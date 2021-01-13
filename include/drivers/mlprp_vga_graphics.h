#ifndef __MLPRP_VGA_GRAPHICS__
#define __MLPRP_VGA_GRAPHICS__

#include "../../libc/mlprp_cdef.h"

volatile char* FRAMEBUFFER;

uint16_t RESX;
uint16_t RESY;

void VGA_PUTCHAR(uint16_t _x, uint16_t _y, char _c);
void VGA_PRINT(char* str, uint8_t colour);

#endif
