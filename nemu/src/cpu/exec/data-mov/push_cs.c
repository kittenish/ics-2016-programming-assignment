#include "cpu/exec/helper.h"
#include "cpu/decode/modrm.h"

make_helper(push_cs){
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp, 4, cpu.cs.val, 3);
	print_asm("push CS");
	return 1;
}
