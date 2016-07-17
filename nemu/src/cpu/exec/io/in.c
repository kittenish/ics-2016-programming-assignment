#include "cpu/exec/helper.h"
#include "device/port-io.h"

make_helper(inb) {
	cpu.al = (uint8_t) pio_read(cpu.dx, 1);
	print_asm("in    %%al,(%%dx)");
	return 1;
}

make_helper(inl) {
	cpu.eax = pio_read(cpu.dx, 4);
	print_asm("in    %%eax,(%%dx)");	
	return 1;
}


