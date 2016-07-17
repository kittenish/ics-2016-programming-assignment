#ifndef __REG_H__
#define __REG_H__

#include "common.h"
#include "../../../lib-common/x86-inc/cpu.h"

enum { R_EAX, R_ECX, R_EDX, R_EBX, R_ESP, R_EBP, R_ESI, R_EDI,EFLAGS };
enum { R_AX, R_CX, R_DX, R_BX, R_SP, R_BP, R_SI, R_DI };
enum { R_AL, R_CL, R_DL, R_BL, R_AH, R_CH, R_DH, R_BH };
enum {eax, ecx, edx, ebx, esp, ebp, esi, edi};
enum {DS, CS, SS, ES};

/* TODO: Re-organize the `CPU_state' structure to match the register
 * encoding scheme in i386 instruction format. For example, if we
 * access cpu.gpr[3]._16, we will get the `bx' register; if we access
 * cpu.gpr[1]._8[1], we will get the 'ch' register. Hint: Use `union'.
 * For more details about the register encoding scheme, see i386 manual.
 */

struct bitfield
{
	unsigned int CF :1;
	unsigned int i1:1;
	unsigned int PF:1;
	unsigned int i3:1;
	unsigned int AF:1;
	unsigned int i5:1;
	unsigned int ZF:1;
	unsigned int SF:1;
	unsigned int TF:1;
	unsigned int IF:1;
	unsigned int DF:1;
	unsigned int OF:1;
	unsigned int TOPL:2;
	unsigned int NT:1;
	unsigned int i15:1;
	unsigned int RF:1;
	unsigned int VM:1;
	unsigned int i18:14;
};
typedef struct CPU_state
{
	union
	{
		uint32_t _32;
		uint16_t _;
		union
		{
			uint16_t _16;
			uint8_t _8[2];
		};
		
	}gpr[8];	
	/* Do NOT change the order of the GPRs' definitions. */
	
	swaddr_t eip;
	
	union
	{
		struct bitfield  EFLAGS;
		uint32_t num;  
	};
	
	union
	{
		uint16_t val;
		struct
		{
			uint32_t rpl : 2;
			uint32_t ti : 1;
			uint32_t index : 13; 
		};
	}ds, ss, cs, es;
	
	union
	{
		struct
		{
			uint32_t val_1;
			uint32_t val_2;
		};
		struct
		{
			uint32_t limit_1 : 16;
			uint32_t base_1 : 16;
			uint32_t base_2 : 8;
			uint32_t a : 1;
			uint32_t type : 3;
			uint32_t s : 1;
			uint32_t dpl : 2;
			uint32_t p : 1;
			uint32_t limit_2 : 4;
			uint32_t avl : 1;
			uint32_t o : 1;
			uint32_t d : 1;
			uint32_t g : 1;
			uint32_t base_3 : 8;
		};
	}gdt[4]; // cs : 0; ds : 1; es : 2; ss :3

	struct
	{
		uint32_t base : 32;
		uint32_t limit : 16;
	}gdtr, idtr;

	CR0 cr0;
	CR3 cr3;
	bool INTR;
} CPU_state;

extern CPU_state cpu;


static inline int  check_reg_index(int index) {
	assert(index >= 0 && index < 8);
	return index;
	}
#define reg_l(index) (cpu.gpr[check_reg_index(index)]._32)
#define reg_b(index) (cpu.gpr[check_reg_index(index)&0x3]._8[index>>2])
#define reg_w(index) (cpu.gpr[check_reg_index(index)]._16)
#define eax gpr[0]._32
#define ecx gpr[1]._32
#define edx gpr[2]._32
#define ebx gpr[3]._32
#define esp gpr[4]._32
#define ebp gpr[5]._32
#define esi gpr[6]._32
#define edi gpr[7]._32
#define ax gpr[0]._16
#define cx gpr[1]._16
#define dx gpr[2]._16
#define bx gpr[3]._16
#define sp gpr[4]._16
#define bp gpr[5]._16
#define si gpr[6]._16
#define di gpr[7]._16
#define al gpr[0]._8[0]
#define ah gpr[0]._8[1]
#define cl gpr[1]._8[0]
#define ch gpr[1]._8[1]
#define dl gpr[2]._8[0]
#define dh gpr[2]._8[1]
#define bl gpr[3]._8[0]
#define bh gpr[3]._8[1]


extern const char* regsl[];
extern const char* regsw[];
extern const char* regsb[];

#endif
