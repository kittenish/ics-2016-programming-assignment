#include "cpu/exec/template-start.h"

#define instr dec

static void do_execute () {
/*	DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);
	if(result==0)				//ZF
		cpu.EFLAGS.ZF=1;
	else cpu.EFLAGS.ZF=0;
	if(result>=0)				//SF
		cpu.EFLAGS.SF=0;
	else cpu.EFLAGS.SF=1;
	if(((result+1) & 0xf) < (result & 0xf))	//AF
		cpu.EFLAGS.AF=1;
	else cpu.EFLAGS.AF=0;
	int i=0,sum=0;				//PF
	for(;i<8;i++)
	{
		if(((result >> i) & 0x1) ==1)
			sum++;
	}
	if(sum%2==0)
		cpu.EFLAGS.PF=1;
	else cpu.EFLAGS.PF=0;
	// TODO: Update EFLAGS. 
	//panic("please implement me");

	print_asm_template1();
	*/
DATA_TYPE result = op_src->val - 1;
	OPERAND_W(op_src, result);

		/* TODO: Update EFLAGS. */
		    uint32_t step = DATA_BYTE * 8 - 1;
			    unsigned sf = 0x1 & (op_src->val >> step);
				    unsigned df = 0x1 & (op_dest->val >> step);
					    unsigned rf = 0x1 & (result >> step);

						    cpu.EFLAGS.OF = (!df && sf && rf) || (df && !sf && !rf);
							    cpu.EFLAGS.SF = rf;
								    cpu.EFLAGS.ZF = (result == 0);

									    result = 0xff & result;
										    unsigned count;
											    for(count = 0; result; ++count)
													        result &= (result - 1);
															    cpu.EFLAGS.PF = !(count % 2);

																	print_asm_template1();

}

make_instr_helper(rm)
#if DATA_BYTE == 2 || DATA_BYTE == 4
make_instr_helper(r)
#endif

#include "cpu/exec/template-end.h"
