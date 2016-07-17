#include "cpu/exec/template-start.h"

#define instr lgdt

static void do_execute()
{
	hwaddr_t addr = instr_fetch(cpu.eip+3,4);
	printf("-----%x----\n",addr);
	if(ops_decoded.is_data_size_16)
	{
		cpu.gdtr.limit = lnaddr_read(addr, 2) & 0x0000ffff;
		cpu.gdtr.base = lnaddr_read(addr + 2, 3) & 0x00ffffff;
	}
	//else 
	{
		cpu.gdtr.limit = lnaddr_read(addr, 2) & 0x0000ffff;
		cpu.gdtr.base = lnaddr_read(addr + 2, 4);
	}
	print_asm_template1();
}

make_instr_helper(rm)

#include "cpu/exec/template-end.h"
