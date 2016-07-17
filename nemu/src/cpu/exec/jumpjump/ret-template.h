#include "cpu/exec/template-start.h"

#define instr ret

static void do_execute()
{
	uint32_t addr=cpu.esp;
	cpu.eip=swaddr_read(addr,4,0)-DATA_BYTE - 1;
	cpu.esp=cpu.esp+4;
	print_asm("ret");
	cpu.esp = cpu.esp + op_src->val;
}

make_instr_helper(i)

make_helper(concat(ret_, SUFFIX))
{
	uint32_t addr=cpu.esp;
	cpu.eip=swaddr_read(addr,4,0) - 1;
	cpu.esp=cpu.esp+4;
	print_asm("ret");
//	printf("----------------------\n\n");
	//printf("%x-- ",cpu.eip);
	// print_asm_template1();
	//if (ops_decoded.instr_len == 3)
		//cpu.eip = cpu.eip - 2;
	return 1;
}


#include "cpu/exec/template-end.h"
