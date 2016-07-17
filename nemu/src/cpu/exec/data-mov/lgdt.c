#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(lgdt_m)
{
	if(instr_fetch(eip + 1, 1) == 0x15)
	{
		//printf("wqsq");
		swaddr_t addr = instr_fetch(cpu.eip + 3, 4);
		cpu.gdtr.limit = swaddr_read(addr, 2, 3);
		cpu.gdtr.base = swaddr_read(addr + 2, 4, 3);
		print_asm("lgdt 0x%x", addr);
		return 6;
	}
	else 
	{
		cpu.gdtr.limit = swaddr_read(cpu.eax, 2, 3);
		cpu.gdtr.base = swaddr_read(cpu.eax + 2, 4, 3);
		print_asm("lgdt (%%eax)");
		return 2;
	}

} 
