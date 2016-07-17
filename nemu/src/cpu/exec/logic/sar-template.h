#include "cpu/exec/template-start.h"

#define instr sar

static void do_execute () {
	DATA_TYPE src = op_src->val;
	DATA_TYPE dest = op_dest->val;
	uint32_t leng =( DATA_BYTE << 3) - 1;
	uint32_t first = dest >> leng;
	//printf("%d ",dest);
	uint8_t count = src & 0x1f;
	dest >>= count;
	int i=0;
	//printf("%d ",dest);
	for (i=0;i<count;i++)
	{
		dest = dest | (first << (leng - i ));
		//printf("%d ",dest);
	}
	//printf("%d ",dest);
	OPERAND_W(op_dest, dest);

/*
	DATA_TYPE src = op_src->val;
	DATA_TYPE_S dest = op_dest->val;

	uint8_t count = src & 0x1f;
	dest >>= count;
	OPERAND_W(op_dest, dest);*/
	print_asm_template2();
}

make_instr_helper(rm_1)
make_instr_helper(rm_cl)
make_instr_helper(rm_imm)

#include "cpu/exec/template-end.h"
