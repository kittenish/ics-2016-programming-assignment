#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(lidt_m)
{
	int judge = instr_fetch(cpu.eip+2,1);
	//printf("%x ",judge);
	if(judge == 0x18)
	{
		cpu.idtr.limit = swaddr_read(cpu.eax, 2,3);
		cpu.idtr.base = swaddr_read(cpu.eax + 2, 4, 3);
		print_asm("lidt (%%eax)");
		return 2;
	}
	else 
	{
		panic("lidt error!");
	}
}

