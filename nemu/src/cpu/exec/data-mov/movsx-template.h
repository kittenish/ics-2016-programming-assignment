#include "cpu/exec/template-start.h"

#define instr movsb
#if DATA_BYTE == 2 || DATA_BYTE == 4
static inline void do_execute() {
	    signed char result;
		if(op_src->type == OP_TYPE_REG)
			result =  reg_b(op_src->reg) ;
		else
			result = op_src->val;
		int index = result;
	    OPERAND_W(op_dest, index);
		print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#define instr movsw
#if DATA_BYTE == 2 || DATA_BYTE == 4
static inline void do_execute() {
#if DATA_BYTE == 2
        panic("no 16 bit movzw instruction");
#endif
	    short result = op_src->val;
	    int index = result;
	    OPERAND_W(op_dest, index);
		print_asm_template2();
}
make_instr_helper(rm2r)
#endif
#undef instr

#include "cpu/exec/template-end.h"
