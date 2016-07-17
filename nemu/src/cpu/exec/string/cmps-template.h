#include "cpu/exec/template-start.h"

#define instr cmps

make_helper(concat(cmps_,SUFFIX))
{
	//printf("sasa");
#if DATA_BYTE == 4
	uint32_t val1 = swaddr_read(cpu.esi, 4, 1);
	uint32_t val2 = swaddr_read(cpu.edi, 4, 2);
	 uint32_t result = val1 - val2;//ES
	// printf("aa");
#else
	 DATA_TYPE_S val1 = swaddr_read(cpu.esi, DATA_BYTE, 1);
	 DATA_TYPE_S val2 = swaddr_read(cpu.edi, DATA_BYTE, 2);
	DATA_TYPE_S result = val1 - val2;
//	printf("bb");
#endif
	uint32_t leng = (DATA_BYTE << 3)-1;
	if(((result >> leng == 1) && (val1 >>leng == 0) && (val2 >>leng == 1))||((result >> leng ==0 ) &&(val1 >> leng == 1)&&(val2 >>leng==0)))                       //OF
		cpu.EFLAGS.OF =1;
	else cpu.EFLAGS.OF=0;
	if (result == 0 )
		cpu.EFLAGS.ZF = 1;
	else cpu.EFLAGS.ZF =0;
	//printf (" %x %x",swaddr_read(cpu.si,1),swaddr_read(cpu.edi,1));
	cpu.EFLAGS.SF = result >> leng;			//SF
	if(((result + val2) & 0xf) < (result & 0xf))	//AF
		cpu.EFLAGS.AF=1;
	else cpu.EFLAGS.AF=0;
	if(val1 < val2)
		cpu.EFLAGS.CF=1;                     //CF
	else cpu.EFLAGS.CF=0;
	int i=0,sum=0;				             //PF
	for(;i<8;i++)
	{
		if(((result >> i) & 0x1) ==1)
			sum++;
	}
	if(sum%2==0)
		cpu.EFLAGS.PF=1;
	else cpu.EFLAGS.PF=0;
	if(cpu.EFLAGS.DF == 0)
	{
#if DATA_BYTE == 4
		cpu.esi = cpu.esi +DATA_BYTE ;
		cpu.edi = cpu.edi + DATA_BYTE;
#else
		cpu.si = cpu.si +DATA_BYTE;
		cpu.di = cpu.di + DATA_BYTE;
#endif
	}
	else 
	{
#if DATA_BYTE == 4
		cpu.esi = cpu.esi -DATA_BYTE;
		cpu.edi = cpu.edi -DATA_BYTE;
#else 
		cpu.si = cpu.si - DATA_BYTE;
		cpu.di = cpu.di - DATA_BYTE;
#endif
	}
	print_asm("cmps");
	return 1;
}



#include "cpu/exec/template-end.h"
