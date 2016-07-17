#include "cpu/exec/template-start.h"

#define instr call

static void do_execute()
{
//	uint32_t leng=0,ceip;
	uint32_t temp = ops_decoded.instr_len;
#if DATA_BYTE==2 
//	leng=3;
#endif

#if DATA_BYTE==4
//	leng=5;
#endif
	REG(R_ESP) = REG(R_ESP)-4;
	op_dest->size = 4;
	op_dest->type = OP_TYPE_MEM;
	op_dest->addr = REG(R_ESP);
	OPERAND_W(op_dest, cpu.eip+temp );
	cpu.eip=op_src->val - ops_decoded.instr_len;
//	ceip=cpu.eip+leng;
//	print_asm_template4();
}

make_instr_helper(rm)

make_helper(concat(call_i_,SUFFIX))
{
	int32_t tmp = instr_fetch(cpu.eip +1,DATA_BYTE);
	REG(R_ESP)= REG(R_ESP)-4;
	op_dest->size = 4;
	op_dest->type = OP_TYPE_MEM;
	op_dest->addr = REG(R_ESP);
	OPERAND_W(op_dest,cpu.eip+5);
	cpu.eip = cpu.eip + tmp ;// DATA_BYTE+1;
	print_asm("call 0x%x",cpu.eip);
	return DATA_BYTE + 1;

}

#include "cpu/exec/template-end.h"
