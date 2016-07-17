#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(leave)
{
	cpu.esp = cpu.ebp;
	cpu.ebp = swaddr_read(cpu.esp,4, 3);
	cpu.esp += 4;
	//printf("%x ",cpu.esp);
	print_asm("leave");
	return 1;
}

