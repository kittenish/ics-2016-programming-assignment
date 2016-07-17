#include "cpu/exec/template-start.h"

#define instr bt

static void do_execute () {
#if DATA_BYTE == 4
	cpu.EFLAGS.CF =	(op_dest->val >> ( op_src->val & 0x1f)) & 0x1;
	//printf("%x %x %x",op_dest->val,cpu.ecx,cpu.EFLAGS.CF);
#else
	cpu.EFLAGS.CF = (op_dest->val >> ( op_src->val & 0xf)) & 0x1;
#endif
	print_asm_template2();
}

make_instr_helper(r2rm)


#include "cpu/exec/template-end.h"
