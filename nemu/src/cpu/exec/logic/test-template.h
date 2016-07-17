#include "cpu/exec/template-start.h"

#define instr test

static void do_execute()
{
	DATA_TYPE result = op_dest->val & op_src->val;
	cpu.EFLAGS.CF=0;
	uint32_t flag = MSB(result) & 0x1;
	//printf(" %x ",result);
	cpu.EFLAGS.OF=0;
	if(flag ==1)
		cpu.EFLAGS.SF= 1;
	else
		cpu.EFLAGS.SF = 0;
	if(result == 0)
		cpu.EFLAGS.ZF=1;
	else cpu.EFLAGS.ZF=0;
	//printf("%x %x ",cpu.edi, cpu.EFLAGS.ZF);
	int sum=0,i=0;
	for(;i<8;i++)
		if(((result >> i) & 0x1)==1 )
			sum++;
	if(sum%2==0)
		cpu.EFLAGS.PF=1;
	else cpu.EFLAGS.PF=0;
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(r2rm)

#include "cpu/exec/template-end.h"
