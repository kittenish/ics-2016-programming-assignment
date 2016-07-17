#include "cpu/exec/template-start.h"

#define instr jecxz

static void do_execute()
{
//	uint32_t ceip,leng=0;
	if( cpu.ecx == 0)
		cpu.eip += (DATA_TYPE_S)op_src->val;
#if DATA_BYTE ==1
//	leng=2;
#endif
//	ceip=cpu.eip+leng;
//	print_asm_template4();
}

make_instr_helper(i)

#include "cpu/exec/template-end.h"
