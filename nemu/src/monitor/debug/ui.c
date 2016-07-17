#include "monitor/monitor.h"
#include "monitor/expr.h"
#include "monitor/watchpoint.h"
#include "nemu.h"
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>
#include<elf.h>

char *getstrtab();
Elf32_Sym *getsymtab();
int getsymtabsize();
void cpu_exec(uint32_t);
int MAX_INSTR_TO_PRINT;
/* We use the ``readline'' library to provide more flexibility to read from stdin. */
char* rl_gets() {
	static char *line_read = NULL;
	if (line_read) {
		free(line_read);
		line_read = NULL;
	}

	line_read = readline("(nemu) ");
	if (line_read && *line_read) {
		add_history(line_read);
	}

	return line_read;
}

static int cmd_c(char *args) {
	cpu_exec(-1);
	return 0;
}

static int cmd_q(char *args) {
	return -1;
}

static int cmd_help(char *args);

static int cmd_si(char *args)
{
	int n=0,i=0;
	char *arg = strtok(NULL, " ");
	if(args!=NULL)
	{{
		
		for(i=0;arg[i]!='\0';i++)
			n=10*n+(arg[i]-48);
	}
	cpu_exec(n);}
	else cpu_exec(1);
	return 0;
}
static int cmd_info(char *args)
{
	char *arg=strtok(args, " ");
	if(strcmp(arg,"r")==0)
	{
		printf("EAX  :  %15x  %8x  %4x  %4x\n",cpu.gpr[0]._32,cpu.gpr[0]._16,cpu.gpr[0]._8[0],cpu.gpr[0]._8[1]);
		printf("ECX  :  %15x  %8x  %4x  %4x\n",cpu.gpr[1]._32,cpu.gpr[1]._16,cpu.gpr[1]._8[0],cpu.gpr[1]._8[1]);
		printf("EDX  :  %15x  %8x  %4x  %4x\n",cpu.gpr[2]._32,cpu.gpr[2]._16,cpu.gpr[2]._8[0],cpu.gpr[2]._8[1]);
		printf("EBX  :  %15x  %8x  %4x  %4x\n",cpu.gpr[3]._32,cpu.gpr[3]._16,cpu.gpr[3]._8[0],cpu.gpr[3]._8[1]);
		printf("ESP  :  %15x  %8x  \n",cpu.gpr[4]._32,cpu.gpr[4]._16);
		printf("EBP  :  %15x  %8x  \n",cpu.gpr[5]._32,cpu.gpr[5]._16);
		printf("ESI  :  %15x  %8x  \n",cpu.gpr[6]._32,cpu.gpr[6]._16);
		printf("EDI  :  %15x  %8x  \n",cpu.gpr[7]._32,cpu.gpr[7]._16);
		printf("EFLAGS  :  %12x\n",cpu.num);
	}

	else if(strcmp(arg,"w")==0)
	{
		WP *p;
		p=gethead();
		if(p==NULL)
			printf("No watchpoint!\n");
		while(p!=NULL)
		{
			printf("Watchpoint %2d  %s\n",p->NO+1,p->expr);
			p=p->next;
		}
	}
	return 0;
}

static int cmd_x(char *args)
{

	uint32_t exp,p;
	int n=0,i=0;
	bool success=true;
	char arg[20];
	for(i=0;args[i]!=' ';i++)
		arg[i]=args[i];
	arg[i]='\0';
	exp=expr(args+i,&success);
	if(success==false)
		{
			printf("Wrong expression!");
			return 0;
		}
	for(i=0;arg[i]!='\0';i++)
		n=10*n+(arg[i]-48);
	for(i=0;i<n;i++)
	{
		if(i%5==0)
			printf("  %x:    ",exp+i);
		p = swaddr_read((exp+i),1,1);
		printf("%3x",p);
		if(i%5==4)
			printf("\n");
		
	}
	printf("\n");
	return 0;
}
static int cmd_p(char *agrs)
{
	bool success=true;
	uint32_t ans;
	ans=expr(agrs,&success);
	if(success==false) 
		printf("Wrong expression!\n");
	else printf("%d\n",ans); 
	return 0;
}

static int cmd_w(char *agrs)
{
	WP *p;
	p=new_wp(agrs);
	bool *success;
	bool anssuccess=true;
	success=&anssuccess;
	p->ans=expr(p->expr,success);
	if(*success==false)
		printf("Check your watchpoint!\n");
	return 0;
}
static int cmd_d(char *args)
{
	int num=0,i=0;
	for(;args[i]!='\0';i++)
		num=num+(args[i]-48);
	free_wp(num);
	return 0;
}
static int cmd_px(char *args)
{
	bool success=true;
	uint32_t ans;
	ans=expr(args,&success);
	if(success==false)
		printf("Wrong expression!\n");
	else
		printf("0x%x\n",ans);
		return 0;
}

static int cmd_bt()
{
	uint32_t flag=swaddr_read(cpu.ebp, 4, 3); 
	uint32_t address=swaddr_read(cpu.ebp+4, 4, 3);
	int i=0;
	for(i=0;i<5;i++)
	{
		if(flag==0)
			printf("#%d  %0xx  in  ",i,cpu.eip);
		else
			printf("#%d  %0xx  in  ",i,address);
		Elf32_Sym *symtab=getsymtab();		
		int size=getsymtabsize();
		char *strtab=getstrtab();
		int j;
		if(flag==0)
			for(j=0;j<size;j++)
			{

				if( cpu.eip>=symtab[j].st_value && cpu.eip <= symtab[j].st_value+symtab[j].st_size)
				{
					 printf("%s ()\n",strtab+symtab[j].st_name);
					 break;
				}
			}
		else
			for(j=0;j<size;j++)
			{
				if( address>=symtab[j].st_value && address <= symtab[j].st_value+symtab[j].st_size)
				{
					printf("%s ()\n",strtab+symtab[j].st_name);
					break;
				}
			}
		if(flag==0)
			break;
		address=swaddr_read(flag+4 , 4, 3);
		flag=swaddr_read(flag, 4, 3);
	}
	return 0;
}

static struct {
	char *name;
	char *description;
	int (*handler) (char *);
} cmd_table [] = {
	{ "help", "Display informations about all supported commands", cmd_help },
	{ "c", "Continue the execution of the program", cmd_c },
	{ "q", "Exit NEMU", cmd_q },
        { "si","The assembly code of instructions executed is only output to the screen when the number of instructions executed is less than N.", cmd_si},
	{"info", "Print status", cmd_info},
	{"x","The memory starting address will be the value of the expression, and the sequent 4*N bytes will be displayed in Hexaecimal",cmd_x},
	{"p","Calculate the expression",cmd_p},
	{"w","Set watchpoint",cmd_w},
	{"d","Delete waychpoint",cmd_d},
	{"p/x","Print status in hexadecimal",cmd_px},
	{"bt","Print a stack frame chain",cmd_bt}
};
	/* TODO: Add more commands */



#define NR_CMD (sizeof(cmd_table) / sizeof(cmd_table[0]))

static int cmd_help(char *args) {
	/* extract the first argument */
	char *arg = strtok(NULL, " ");
	int i;

	if(arg == NULL) {
		/* no argument given */
		for(i = 0; i < NR_CMD; i ++) {
			printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
		}
	}
	else {
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(arg, cmd_table[i].name) == 0) {
				printf("%s - %s\n", cmd_table[i].name, cmd_table[i].description);
				return 0;
			}
		}
		printf("Unknown command '%s'\n", arg);
	}
	return 0;
}

void ui_mainloop() {
	while(1) {
		char *str = rl_gets();
		char *str_end = str + strlen(str);

		/* extract the first token as the command */
		char *cmd = strtok(str, " ");
		if(cmd == NULL) { continue; }

		/* treat the remaining string as the arguments,
		 * which may need further parsing
		 */
		char *args = cmd + strlen(cmd) + 1;
		if(args >= str_end) {
			args = NULL;
		}

#ifdef HAS_DEVICE
		extern void sdl_clear_event_queue(void);
		sdl_clear_event_queue();
#endif

		int i;
		for(i = 0; i < NR_CMD; i ++) {
			if(strcmp(cmd, cmd_table[i].name) == 0) {
				if(cmd_table[i].handler(args) < 0) { return; }
				break;
			}
		}

		if(i == NR_CMD) { printf("Unknown command '%s'\n", cmd); }
	}
}
