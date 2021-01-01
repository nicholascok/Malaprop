#include "../include/kernel.h"

struct string {
	char* val;
	int len;
};

extern void kmain(void) {
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
}
