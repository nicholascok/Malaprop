#ifndef __IDT_H__
#define __IDT_H__

#define PIC1	0x20
#define PIC2	0xA0
#define EOI	0x20

#include "../libc/mlprp_cdef.h"
#include "../include/drivers/mlprp_vga_text.h"

uint16_t IDT_DEFAULT_SELECTOR;
uint8_t GATE_INTERRUPT;
uint8_t GATE_TRAP;
uint8_t GATE_TASK;

struct IDT_ENTRY {
	uint16_t offset_1;
	uint16_t selector;
	uint8_t nullspace;
	uint8_t type_attr;
	uint16_t offset_2;
} __attribute__((packed));

struct IDT_ENTRY IDT[256];

void IDT_ADD_DESCRIPTOR(uint32_t _ptr, uint8_t _type_attr);

void IDT_INIT(void);

void LOAD_IDT(void* _ptr, uint16_t _len);

void IRQ_DEFAULT_HANDLER(void);

#endif
