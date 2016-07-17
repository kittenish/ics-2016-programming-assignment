#include "cpu/exec/template-start.h"

#define instr or

static void do_execute () {
	DATA_TYPE result = op_dest->val | op_src->val;
	OPERAND_W(op_dest, result);

	/* TODO: Update EFLAGS. */
	cpu.EFLAGS.CF = 0;
	cpu.EFLAGS.OF = 0;
	cpu.EFLAGS.ZF = !result;
	uint32_t leng = (DATA_BYTE << 3)-1;
	cpu.EFLAGS.SF = (result >> leng) & 0x1;
	int i=0,sum=0;
	for(;i<8;i++)
	{
		if(((result >> i) & 0x1 ) == 1)
			sum++;
	}
	if(sum%2==0)
		cpu.EFLAGS.PF = 1;
	else cpu.EFLAGS.PF = 0;

	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(si2rm)
#endif
make_instr_helper(r2rm)
make_instr_helper(rm2r)

#include "cpu/exec/template-end.h"
