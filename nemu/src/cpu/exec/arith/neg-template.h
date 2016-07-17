#include "cpu/exec/template-start.h"

#define instr neg

static void do_execute() {
	DATA_TYPE result = -op_src->val;
	OPERAND_W(op_src, result);
	if(result == 0)
		cpu.EFLAGS.CF = 0;
	else 
		cpu.EFLAGS.CF = 1;

	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
