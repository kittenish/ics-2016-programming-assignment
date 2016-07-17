#include "cpu/exec/template-start.h"
#define instr push

static void do_execute()
{
#if DATA_BYTE == 2
	cpu.esp = cpu.esp-2;
	swaddr_write(cpu.esp , 2, op_src->val, 3);
#elif DATA_BYTE == 1
	//printf("%xaa ",cpu.esp); 
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp , 4 ,op_src->val & 0xff, 3);//SS
#else 
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp , 4 ,op_src->val, 3);//SS
#endif
	print_asm_template1();
}

make_instr_helper(r);
make_instr_helper(i);
make_instr_helper(rm);
#include "cpu/exec/template-end.h"
