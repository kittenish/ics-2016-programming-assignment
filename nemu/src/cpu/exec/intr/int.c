#include "cpu/exec/helper.h"

extern void raise_intr(uint8_t NO);

make_helper(int_i_b) {
	uint32_t number = instr_fetch(eip + 1, 1);
	print_asm("int 0x%02x", number);
	//printf("int");
	cpu.eip = cpu.eip + 2; 
	raise_intr(number);
	assert(0);
	return 2;
}

