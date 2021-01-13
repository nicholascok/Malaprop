#ifndef __KERNEL__
#define __KERNEL__

#include "../libc/mlprp_cdef.h"
#include "./drivers/mlprp_vga_text.h"
#include "./idt.h"

void kmain(void);

void irq_keyboard(void);

uint8_t get_ascii(uint8_t _c);

#endif
