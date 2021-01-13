#include "../include/kernel.h"

struct string {
	char* val;
	int len;
};

void kmain(void) {
	char input[] = "root";
	struct string USER = {input, 4};
	char intpr[USER.len + 3];
	intpr[0] = '<';
	for (int i = 1; i <= USER.len; i++) {
		intpr[i] = USER.val[i - 1];
	}
	intpr[USER.len + 1] = '>';
	intpr[USER.len + 2] = ' ';
	VGA_CLEAR();
	VGA_PRINT(intpr, VGA_COLOUR);
	VGA_CURSOR_ENABLE(11, 12);
	IDT_INIT();
	IDT_SET_DESCRIPTOR(33, irq_keyboard, GATE_INTERRUPT);
	asm("STI");
}

void irq_keyboard(void) {
	__asm__("CLI; PUSHA");
	char key[2] = {get_ascii(((char) inb(0x60)) & 0xFF), 0};
	VGA_PRINT(key, VGA_COLOUR);
	outb(PIC2, EOI);
	outb(PIC1, EOI);
	__asm__("POPA; STI; LEAVE; IRET");
}

uint8_t get_ascii(uint8_t _c) {
	switch (_c) {
		case 0x1E: //A
			return 0x41;
		case 0x30: //B
			return 0x42;
		case 0x2E: //C
			return 0x43;
		case 0x20: //D
			return 0x44;
		case 0x12: //E
			return 0x45;
		case 0x21: //F
			return 0x46;
		case 0x22: //G
			return 0x47;
		case 0x23: //H
			return 0x48;
		case 0x17: //I
			return 0x49;
		case 0x24: //J
			return 0x4A;
		case 0x25: //K
			return 0x4B;
		case 0x26: //L
			return 0x4C;
		case 0x32: //M
			return 0x4D;
		case 0x31: //N
			return 0x4E;
		case 0x18: //O
			return 0x4F;
		case 0x19: //P
			return 0x50;
		case 0x10: //Q
			return 0x51;
		case 0x13: //R
			return 0x52;
		case 0x1F: //S
			return 0x53;
		case 0x14: //T
			return 0x54;
		case 0x16: //U
			return 0x55;
		case 0x2F: //V
			return 0x56;
		case 0x11: //W
			return 0x57;
		case 0x2D: //X
			return 0x58;
		case 0x15: //Y
			return 0x59;
		case 0x2C: //Z
			return 0x5A;
		case 0x39: //space
			return 0x20;
		case 0x0E: //backspace
			return 0x08;
		default:
			return 0x00;
	}
}

