#include "cpu/exec/helper.h"

uint16_t pop_16()
{
	uint16_t  val = swaddr_read(cpu.esp, 2, 3);
	cpu.esp = cpu.esp + 2;
	return val;
}

uint32_t pop_32()
{
	uint32_t val = swaddr_read(cpu.esp, 4, 3);
	cpu.esp = cpu.esp + 4;
	return val;
}

make_helper(popa_v)
{
	/*if(ops_decoded.is_data_size_16)
	{
		cpu.di = pop_16();
		cpu.si = pop_16();
		cpu.bp = pop_16();
		pop_16();
		cpu.bx = pop_16();
		cpu.dx = pop_16();
		cpu.cx = pop_16();
		cpu.ax = pop_16();
	}
	else
	{*/
		cpu.edi = pop_32();
		cpu.esi = pop_32();
		cpu.ebp = pop_32();
		cpu.esp = cpu.esp + 4;
		cpu.ebx = pop_32();
		cpu.edx = pop_32();
		cpu.ecx = pop_32();
		cpu.eax = pop_32();
//	}
	print_asm("popa");
	return 1;
}
