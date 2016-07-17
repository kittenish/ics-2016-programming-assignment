#include "cpu/exec/helper.h"

make_helper(jmp_rel_b) {
	int32_t tmp = instr_fetch(cpu.eip + 1, 1);
	cpu.eip += tmp << 24 >> 24;
	print_asm("jmp 0x%x", cpu.eip + 2);
	return 2;
}

make_helper(jmp_rel_w) {
	int32_t tmp = instr_fetch(cpu.eip + 1, 2);
	cpu.eip += tmp << 16 >> 16;
	print_asm("jmp 0x%x", cpu.eip + 3);
	return 3;
}

make_helper(jmp_rel_l) {
	cpu.eip += instr_fetch(cpu.eip + 1, 4);
	print_asm("jmp 0x%x", cpu.eip + 5);
	return 5;
}


#define DATA_BYTE 1
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 2
#include "jmp-template.h"
#undef DATA_BYTE

#define DATA_BYTE 4
#include "jmp-template.h"
#undef DATA_BYTE

make_helper_v(jmp_seg);
make_helper_v(jmp_rel);
make_helper_v(jmp_rm);
