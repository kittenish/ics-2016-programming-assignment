#include "cpu/exec/template-start.h"


make_helper(concat(cld_,SUFFIX)) 
{
	cpu.EFLAGS.DF = 0;
	print_asm("cld");
	return 1;
}


#include "cpu/exec/template-end.h"
