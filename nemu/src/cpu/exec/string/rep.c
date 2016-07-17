#include "cpu/exec/helper.h"

make_helper(exec);

make_helper(rep) {
	int len;
	int count = 0;
	if(instr_fetch(eip + 1, 1) == 0xc3) {
		/* repz ret */
		exec(eip + 1);
		len = 0;
	}
	else {
		//printf("%x ",cpu.ecx);
		//cpu.EFLAGS.ZF = 1;
		while(cpu.ecx) {
			//printf("%d",eip+1);
			exec(eip + 1);
			count ++;
			cpu.ecx --;
			//printf("%x", ops_decoded.opcode);
			assert(ops_decoded.opcode == 0xa4	// movsb
				|| ops_decoded.opcode == 0xa5	// movsw
				|| ops_decoded.opcode == 0xaa	// stosb
				|| ops_decoded.opcode == 0xab	// stosw
				|| ops_decoded.opcode == 0xa6	// cmpsb
				|| ops_decoded.opcode == 0xa7	// cmpsw
				|| ops_decoded.opcode == 0xae	// scasb
				|| ops_decoded.opcode == 0xaf	// scasw
				);
			if (ops_decoded.opcode == 0xa6 || ops_decoded.opcode == 0xa7||  ops_decoded.opcode == 0xae || ops_decoded.opcode == 0xaf)
			{
				if (cpu.EFLAGS.ZF == 0)
					break;	
			}

			/* TODO: Jump out of the while loop if necessary. */
		}
		len = 1;
		/*if(ops_decoded.opcode == 0xa4)
			printf("movsb\n");
		else if(ops_decoded.opcode == 0xa5)
			printf("movsw\n");
		else if(ops_decoded.opcode == 0xaa)
			printf("stosb\n");
		else if(ops_decoded.opcode == 0xab)
			printf("stosw\n");
		else if(ops_decoded.opcode == 0xa6)
			printf("cmpsb\n");
		else if(ops_decoded.opcode == 0xa7)
			printf("cmpsw\n");
		else if(ops_decoded.opcode == 0xae)
			printf("scacb\n");
		else
			printf("scasw\n %x",ops_decoded.opcode);*/
	}

#ifdef DEBUG
	char temp[80];
	sprintf(temp, "rep %s", assembly);
	sprintf(assembly, "%s[cnt = %d]", temp, count);
#endif
	return len + 1;
}
