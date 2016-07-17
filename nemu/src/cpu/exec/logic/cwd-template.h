#include "cpu/exec/template-start.h"


make_helper(concat(cwd_,SUFFIX)) 
{
#if DATA_BYTE == 2
	    signed char index = cpu.al;
		cpu.ax = (short) index;
	    print_asm("cbw");
#elif DATA_BYTE == 4
		short index = cpu.ax;
	    cpu.eax = (int) index;
		print_asm("cwde");
#endif
	    return 1;
}


#include "cpu/exec/template-end.h"
