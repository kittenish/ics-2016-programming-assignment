#include "cpu/exec/template-start.h"

#define instr stos

make_helper(concat(stos_,SUFFIX))
{
#if DATA_BYTE == 4 
	swaddr_write(cpu.edi,4, cpu.eax, 2);//ES
#endif

#if DATA_BYTE == 1
	swaddr_write(cpu.edi ,1, cpu.gpr[0]._8[0], 2);
#endif

#if DATA_BYTE == 2 
	swaddr_write(cpu.edi,2,cpu.ax,2);// cpu.gpr[0]._16, 2);
#endif

	if(cpu.EFLAGS.DF == 0)
	{
#if DATA_BYTE == 4
		cpu.edi = cpu.edi + DATA_BYTE;
#else
		cpu.di = cpu.di +DATA_BYTE;
#endif
	}
	else
	{
#if DATA_BYTE == 4
		cpu.edi = cpu.edi - DATA_BYTE;
#else
		cpu.di = cpu.di - DATA_BYTE;
#endif
	}			
	print_asm("stos");
	return 1;
}


#include "cpu/exec/template-end.h"
