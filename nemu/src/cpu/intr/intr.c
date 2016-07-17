#include <setjmp.h>
#include "common.h"
#include "cpu/exec/helper.h"

extern jmp_buf jbuf;

void raise_intr(uint8_t NO) {
	/* TODO: Trigger an interrupt/exception with ``NO''.
	* That is, use ``NO'' to index the IDT.
	*/
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp , 4 ,cpu.num, 3);
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp , 4 ,cpu.cs.val, 3);
	cpu.esp = cpu.esp - 4;
	swaddr_write(cpu.esp , 4 ,cpu.eip, 3);
	unsigned long long desc;
	uint32_t desc_data[2];
	uint32_t val1 = cpu.idtr.base + NO * 8;
	desc_data[0] = lnaddr_read(val1, 4);
	desc_data[1] = lnaddr_read(val1 + 4, 4);
	memcpy(&desc, desc_data, 8);
	uint32_t cs = ((desc) >> 16) & 0xffff;
	uint32_t ceip = ((desc) & 0xffff) | (((desc) >> 32) & 0xffff0000);
	//printf("%llx %x",desc,desc_data[0]);
	cpu.eip = ceip;
	cpu.cs.val = cs;
					
	/* Jump back to cpu_exec() */
	longjmp(jbuf, 1);
}
