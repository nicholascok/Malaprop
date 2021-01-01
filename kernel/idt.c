uint16_t IDT_DEFAULT_SELECTOR = 0x08;
const void* IDT_PTR;

struct IDT_ENTRY {
	uint16_t offset_1;
	uint16_t selector;
	uint8_t null;
	uint8_t type_attr;
	uint16_t offset_2;
};

struct IDT_ENTRY IDT[256]

void IDT_ADD_DESCRIPTOR(void* ptr, uint8_t type_attr) {
	struct IDT_ENTRY irq = {(ptr & 0xFF)}
}
