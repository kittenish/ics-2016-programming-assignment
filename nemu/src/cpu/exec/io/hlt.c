#include "nemu.h"
#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(hlt)
{
	if(!(cpu.EFLAGS.IF & cpu.INTR))
	{
		cpu.eip = cpu.eip - 1;
	}
	print_asm("hlt");
	return 1;
}
