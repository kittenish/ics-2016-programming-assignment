#include "cpu/exec/template-start.h"

#define instr movs

make_helper(concat(movs_,SUFFIX))
{
#if DATA_BYTE == 4
	uint32_t result = swaddr_read(cpu.esi , 4, 1);//DS
	//cpu.esi = cpu.edi;
	//cpu.edi = result;
	swaddr_write(cpu.edi , 4,result, 2);
#else 
	uint16_t result = swaddr_read(cpu.esi , DATA_BYTE, 1);
	swaddr_write(cpu.edi , DATA_BYTE , result, 2);
#endif

	if(cpu.EFLAGS.DF == 0)
	{
#if DATA_BYTE == 4
		cpu.esi = cpu.esi + DATA_BYTE;
		cpu.edi = cpu.edi + DATA_BYTE;
#else
		cpu.si = cpu.si +DATA_BYTE;
		cpu.di = cpu.di +DATA_BYTE;
#endif
	}
	else
	{
#if DATA_BYTE == 4
		cpu.edi = cpu.edi - DATA_BYTE;
		cpu.esi = cpu.esi - DATA_BYTE;
#else
		cpu.si =cpu.si -DATA_BYTE;
		cpu.di = cpu.di -DATA_BYTE;
#endif
	}
	print_asm("movs");			
	//print_asm_template2();
	return 1;
}


#include "cpu/exec/template-end.h"
