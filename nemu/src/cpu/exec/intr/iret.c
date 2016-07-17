#include "cpu/exec/helper.h"

/*uint32_t pop_32()
{
	uint32_t val = swaddr_read(cpu.esp, 4, 3);
	cpu.esp = cpu.esp + 4;
	return val;
}*/
extern uint32_t pop_32();

make_helper(iret) {
	uint32_t p_eip = pop_32();
	uint32_t p_cs = pop_32();
	uint32_t p_EFLAGS = pop_32();

	cpu.cs.val = p_cs;
	cpu.eip = p_eip - 1;
	cpu.num = p_EFLAGS;
	print_asm("iret");
	return 1;
}
