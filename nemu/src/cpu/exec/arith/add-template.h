#include "cpu/exec/template-start.h"

#define instr add

static void do_execute()
{
	//printf("%x ",cpu.esp);
	uint32_t leng=(DATA_BYTE << 3)-1;
	DATA_TYPE result;
	result= op_dest->val + op_src->val;
	if(((result >> leng == 1) && (op_dest->val >>leng == 0) && (op_src->val>> leng == 0))||((result >> leng ==0 ) &&(op_dest->val >> leng == 1)&&(    op_src->val>> leng ==1) ))                                     //OF
		cpu.EFLAGS.OF =1;
	else cpu.EFLAGS.OF=0;
	cpu.EFLAGS.ZF= !(result);               //ZF
	cpu.EFLAGS.SF = MSB(result );			//SF
	if((DATA_TYPE)op_dest->val>(DATA_TYPE)(~(op_src->val)))
	{
		cpu.EFLAGS.CF=1;
	}
	else
	{
		cpu.EFLAGS.CF=0;
	}
	int i=0,sum=0;				             //PF
	for(;i<8;i++)
	{
		if(((result >> i) & 0x1) ==1)
			sum++;
	}
	if(sum%2==0)
		cpu.EFLAGS.PF=1;
	else cpu.EFLAGS.PF=0;	
	OPERAND_W(op_dest,result);
	print_asm_template2();
}

make_instr_helper(i2a)
make_instr_helper(i2rm)
make_instr_helper(rm2r)
make_instr_helper(r2rm)

#if DATA_BYTE==2 ||DATA_BYTE == 4
make_instr_helper(si2rm)
#endif

#include "cpu/exec/template-end.h"
