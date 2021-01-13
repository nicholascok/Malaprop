#include "../include/idt.h"

uint16_t IDT_DEFAULT_SELECTOR = 0x08;
uint8_t GATE_INTERRUPT = 0x8E;
uint8_t GATE_TRAP = 0x8F;
uint8_t GATE_TASK = 0x95;

void IDT_SET_DESCRIPTOR(uint8_t _pos, uint32_t _ptr, uint8_t _type_attr) {
	struct IDT_ENTRY desc = {(_ptr & 0xFFFF), IDT_DEFAULT_SELECTOR, 0, _type_attr, ((_ptr >> 16) & 0xFFFF)};
	IDT[_pos] = desc;
}


void IDT_INIT(void) {
	// remap the interrupt controller so no interrupts overlap (bad hardware design)
	outb(0x20, 0x11);
	outb(0xA0, 0x11);
	outb(0x21, 0x20);
	outb(0xA1, 0x28);
	outb(0x21, 0x04);
	outb(0xA1, 0x02);
	outb(0x21, 0x01);
	outb(0xA1, 0x01);
	outb(0x21, 0x00);
	outb(0xA1, 0x00);
	
	for (int i = 0; i < 16; i++) {
		IDT_SET_DESCRIPTOR(32 + i, IRQ_DEFAULT_HANDLER, GATE_INTERRUPT);
	}
	
	LOAD_IDT(&IDT, sizeof(IDT));
}

void LOAD_IDT(void* _ptr, uint16_t _len) {
	struct {
		uint16_t len;
		void* ptr;
	} __attribute__((packed)) IDTR = {_len, _ptr};
	asm("LIDT %0" :: "m"(IDTR));
}

void IRQ_DEFAULT_HANDLER(void) {
	__asm__("PUSHA");
	outb(PIC2, EOI);
	outb(PIC1, EOI);
	__asm__("POPA; LEAVE; IRET");
}

