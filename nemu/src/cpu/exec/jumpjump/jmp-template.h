#include "cpu/exec/template-start.h"

#define instr jmp

static void do_execute() {
	//printf("%xaa %xaa ",cpu.eax,op_src->val);
	cpu.eip = op_src->val - ops_decoded.instr_len;
	print_asm("jmpx 0x%x", op_src->val);
}

make_instr_helper(rm)

make_helper(concat(jmp_seg_, SUFFIX))
{
	uint32_t ceip = instr_fetch(eip + 1, 4);
	//cpu.eip = ceip ;
	//printf("%x\n",ceip);
	uint32_t cs = instr_fetch(cpu.eip + 5, 2);
//	uint32_t cs = 0;
	cpu.eip = ceip - 7;
#if DATA_BYTE == 2
	cpu.eip = cpu.eip & 0x0000ffff;
#endif
	cpu.cs.val = cs;
	uint32_t addr = cpu.cs.index * 8 + cpu.gdtr.base;
	cpu.gdt[0].limit_1 = lnaddr_read(addr, 2);
	cpu.gdt[0].base_1 = lnaddr_read(addr + 2, 2);
	cpu.gdt[0].base_2 = lnaddr_read(addr + 4, 1);
	cpu.gdt[0].limit_2 = lnaddr_read(addr + 6, 1) & 0xf;
	cpu.gdt[0].base_3 = lnaddr_read(addr + 7, 1);
	print_asm("ljmp $0x%x,0x%x", cpu.cs.val, cpu.eip);
	return 7;
}

#include "cpu/exec/template-end.h"
