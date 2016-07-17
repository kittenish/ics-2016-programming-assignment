#include "cpu/exec/template-start.h"

#define instr cltd

make_helper(concat(cltd_, SUFFIX)) 
{
#if DATA_BYTE == 2
	short index = cpu.ax;
	if(index < 0)
		cpu.dx = 0xffff;
	else cpu.dx = 0;
#else
	int index = cpu.eax;
	if(index < 0)
		cpu.edx = 0xffffffff;
	else 
		cpu.edx = 0;
#endif 
	print_asm("cltd");
	return 1;
}


#include "cpu/exec/template-end.h"
