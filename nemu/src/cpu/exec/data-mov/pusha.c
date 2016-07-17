#include "cpu/exec/helper.h"

void push_16(uint16_t value)
{
	cpu.esp = cpu.esp - 2;
	swaddr_write(cpu.esp, 2, value, 3);
}

void push_32(uint32_t value)
{
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp, 4, value, 3);
}


make_helper(pusha_v)
{
	/*if(ops_decoded.is_data_size_16)
	{
		uint16_t temp = cpu.sp;
		push_16(cpu.ax);
		push_16(cpu.cx);
		push_16(cpu.dx);
		push_16(cpu.bx);
		push_16(temp);
		push_16(cpu.bp);
		push_16(cpu.si);
		push_16(cpu.di);
	}
	else 
	{*/
		uint32_t temp = cpu.esp;
		push_32(cpu.eax);
		push_32(cpu.ecx);
		push_32(cpu.edx);
		push_32(cpu.ebx);
		push_32(temp);
		push_32(cpu.ebp);
		push_32(cpu.esi);
		push_32(cpu.edi);
	//}
	print_asm("pusha");
	return 1;
}
