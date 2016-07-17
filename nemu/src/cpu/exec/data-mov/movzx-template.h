#include "cpu/exec/template-start.h"

#define instr movzb
#if DATA_BYTE == 2 || DATA_BYTE == 4
static inline void do_execute() {
	//printf("aaa ");
	unsigned char result;
	if( op_src->type == OP_TYPE_REG )
		result =  reg_b(op_src->reg) ;
	else 
		result =  op_src->val;
	uint32_t index = result;
	//printf("op_src->reg=%x %p\n", op_src->reg, op_src);
	OPERAND_W(op_dest, index);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movzw
#if DATA_BYTE == 2 || DATA_BYTE == 4
static inline void do_execute() {
#if DATA_BYTE == 2
    panic("no 16 bit movzw instruction");
#endif
	//printf("21212  ");
	unsigned short result = op_src->val;
	unsigned int val = result;
	OPERAND_W(op_dest, val);
	print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#include "cpu/exec/template-end.h"
