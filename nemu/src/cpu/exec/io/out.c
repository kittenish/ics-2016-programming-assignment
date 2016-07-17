#include "cpu/exec/helper.h"
#include "device/port-io.h"

make_helper(outb) {
	pio_write(cpu.dx, 1, (uint32_t) cpu.al);
	print_asm("out    %%al,(%%dx)");
	return 1;
}

make_helper(outl) {
	pio_write(cpu.dx, 4, cpu.eax);
	print_asm("out    %%eax,(%%dx)");
	return 1;
}

