#include "nemu.h"

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <sys/types.h>
#include <regex.h>
#include<elf.h>

char *getstrtab();
Elf32_Sym *getsymtab();
int getsymtabsize();

enum symbol{
	NOTYPE = 256,EQ,NEQ,AND,OR,NUM,NOT,DEREF,MINUS

	/* TODO: Add more token types */

};

static struct rule {
	char *regex;
	int token_type;
} rules[] = {

	/* TODO: Add more rules.
	 * Pay attention to the precedence level of different rules.
	 */

	{" +", NOTYPE},				// spaces
	{"\\+", '+'},				// plus
	{"-", '-'},
	{"\\*",'*'},
	{"\\/", '/'},
	{"\\(", '('},
	{"\\)", ')'},
	{"==", EQ},
	{"[0-9$a-zA-z_]+",NUM},
	{"&&",AND},
	{"!=",NEQ},
	{"\\|+",OR},
	{"!",NOT}
};

#define NR_REGEX (sizeof(rules) / sizeof(rules[0]) )

static regex_t re[NR_REGEX];

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
	int i;
	char error_msg[128];
	int ret;

	for(i = 0; i < NR_REGEX; i ++) {
		ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
		if(ret != 0) {
			regerror(ret, &re[i], error_msg, 128);
			Assert(ret == 0, "regex compilation failed: %s\n%s", error_msg, rules[i].regex);
		}
	}
}

typedef struct token {
	int type;
	char str[64];
} Token;

Token tokens[32];
int nr_token;
static bool make_token(char *e) {
	int position = 0;
	int i;
	regmatch_t pmatch;
	nr_token = 0;
	while(e[position] != '\0') {
		/* Try all rules one by one. */
		for(i = 0; i < NR_REGEX; i ++) {
			if(regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
		//		char *substr_start = e + position;
				int substr_len = pmatch.rm_eo;

			//	Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s", i, rules[i].regex, position, substr_len, substr_len, substr_start);
				

				/* TODO: Now a new token is recognized with rules[i]. Add codes
				 * to record the token in the array ``tokens''. For certain 
				 * types of tokens, some extra actions should be performed.
				 */
			
				switch(rules[i].token_type) {
					case'+':
						tokens[nr_token].type='+';
						nr_token++;
						break;
					case'-':
						tokens[nr_token].type='-';
						nr_token++;
						break;
					case'*':
						tokens[nr_token].type='*';
						nr_token++;
						break;
					case'/':
						tokens[nr_token].type='/';
						nr_token++;
						break;
					case'(':
						tokens[nr_token].type='(';
						nr_token++;
						break;
					case')':
						tokens[nr_token].type=')';
						nr_token++;
						break;
					case NUM:
						tokens[nr_token].type=NUM;
						int j;
						for(j=0;j<substr_len;j++)
							tokens[nr_token].str[j]=e[position+j];
						tokens[nr_token].str[j]='\0';
						nr_token++;
						break;
					case EQ:
						tokens[nr_token].type=EQ;
						nr_token++;
						break;
					case NOTYPE:
						break;
					case AND:
						tokens[nr_token].type=AND;
						nr_token++;
						break;
					case OR:
						tokens[nr_token].type=OR;
						nr_token++;
						break;
					case NEQ:
						tokens[nr_token].type=NEQ;
						nr_token++;
						break;
					case NOT:
						tokens[nr_token].type=NOT;
						nr_token++;
						break;
					default: panic("please implement me");
				}
			position=position+substr_len;
			break;

			}
		}

		if(i == NR_REGEX) {
		printf("no match at position %d\n%s\n%*.s^\n", position, e, position,"");
			return false;
		} 
	
	}
	for(i = 0; i < nr_token; i ++) 
	{
		if(tokens[i].type == '*' && (i == 0 || tokens[i - 1].type == '+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/'||tokens[i-1].type==EQ||tokens[i-1].type==NEQ||tokens[i-1].type==AND||tokens[i-1].type==OR||tokens[i-1].type==NOT) ) 
		{
			tokens[i].type = DEREF;
		}
	}
	for(i = 0; i < nr_token; i ++)
        {
                if(tokens[i].type == '-' && (i == 0 || tokens[i - 1].type == '+'||tokens[i-1].type=='-'||tokens[i-1].type=='*'||tokens[i-1].type=='/'||tokens[i-1].type==EQ||tokens[i-1].type==NEQ||tokens[i-1].type==AND||tokens[i-1].type==OR||tokens[i-1].type==NOT) )
                {
                        tokens[i].type = MINUS;
                }
	}
	return true; 
}
static int ilir=0;
bool check_parentheses(int p, int q)
{
	int il=0,ir=0,i;
	for(i=p;i<q+1;i++)
	{
		if(tokens[i].type=='(') il++;
		if(tokens[i].type==')') ir++;
	}
	if(il!=ir)
	{
		ilir=1;
		printf("Wrong ()!\n");
		return false;
	}
	il=0;
	ir=0;
	i=p;
	if(tokens[p].type=='('&&tokens[q].type==')')
	{
		for(;i<q+1;i++)
		{
			if(tokens[i].type=='(') il++;
			if(tokens[i].type==')') il--;
			if(il<=0&&i!=q) return false;
		}
	return true;
	}
	return false;
}
static int wrongnum=0;
static int wrongreg=0;
uint32_t eval(char *e,int p,int q) {
	uint32_t val1,val2,n=0;
	int i=0,op=0,flag=0;
	int op_type=0;
	if(p > q) {
	  	assert(p > q);
		/* Bad expression */
	}
	else if(p == q)
	 {
		/* Single token.
		 * For now this token should be a number. 
		 * Return the value of the number.
		 */
		if(tokens[p].str[1]!='x'&&tokens[p].type==NUM&&(tokens[p].str[0]=='0'||tokens[p].str[0]=='1'||tokens[p].str[0]=='2'||tokens[p].str[0]=='3'||tokens[p].str[0]=='4'||tokens[p].str[0]=='5'||tokens[p].str[0]=='6'||tokens[0].str[0]=='7'||tokens[p].str[0]=='8'||tokens[p].str[0]=='9'))
		{
			for(i=0;tokens[p].str[i]!='\0';i++)	
			{
				n=n*10+(tokens[p].str[i]-48);
				if(tokens[p].str[i]>57||tokens[p].str[i]<48)
				{
					printf("Wrong number at position %d\n",p);
					wrongnum=1;
					return 0;
				}
			}
			return n;
		}
		else if(tokens[p].str[0]=='x')
		{
			for(i=1;tokens[p].str[i]!='\0';i++)
			{
				n=n*8+(tokens[p].str[i]-48);
				if(tokens[p].str[i]>55||tokens[p].str[i]<48)
				{
					printf("Wrong number at position %d\n",p);
					wrongnum=1;
					return 0;
				}
			}
			return n;
		}
		else if(tokens[p].str[0]=='0'&&tokens[p].str[1]=='x')
		{	
			int num=0;
			for(i=2;tokens[p].str[i]!='\0';i++)
			{
				switch(tokens[p].str[i])
				{
					case '0':
					case '1':
					case '2':
					case '3':
					case '4':
					case '5':
					case '6':
					case '7':
					case '8':
					case '9':
						num=tokens[p].str[i]-48;
						break;
					case 'a':
						num=10;
						break;
					case 'b':
						num=11;
						break;
					case 'c':
						num=12;
						break;
					case 'd':
						num=13;
						break;
					case 'e':
						num=14;
						break;
					case 'f':
						num=15;
						break;
				}
				if(num<0||num>15)
				{
					wrongnum=1;
					printf("Wrong number at position %d\n",p);
					return 0;
				}				
				n=n*16+num;
			}
			return n;
		}
		else if(tokens[p].str[0]=='$')
		{
			int i,len;
			for(i=1;tokens[p].str[i]!='\0';i++);
			len=i-1;
			if(len==3)
			{
				switch(tokens[p].str[2])
				{
					case 'a':
						if(tokens[p].str[1]!='e'||tokens[p].str[3]!='x')
						{
							wrongreg=1;
							return 0;
						}
						else return cpu.eax;
					case 'c':
						if(tokens[p].str[1]!='e'||tokens[p].str[3]!='x')
						{
							wrongreg=1;
							return 0;
						}
						return cpu.ecx;
					case 'd':
						if(tokens[p].str[1]=='e'&&tokens[p].str[3]=='x')
							return cpu.edx;
						else if(tokens[p].str[1]=='e'&&tokens[p].str[3]=='i')
							return cpu.edi;
						else
						{
							wrongreg=1;
							return 0;
						}
					case 'b':
						if(tokens[p].str[1]=='e'&&tokens[p].str[3]=='x')
							return cpu.ebx;
						else if(tokens[p].str[1]=='e'&&tokens[p].str[3]=='p')
							return cpu.ebp;
						else 
						{
							wrongreg=1;
							return 0;
						}
					case 's':
						if(tokens[p].str[3]=='p'&&tokens[p].str[1]=='e')
							return cpu.esp;
						else if(tokens[p].str[3]=='i'&&tokens[p].str[1]=='e')
							return cpu.esi;
						else
						{
							wrongreg=1;
							return 0;
						}
					case 'i':
						if(tokens[p].str[3]=='p'&&tokens[p].str[1]=='e')
							return cpu.eip;
						else
						{
							wrongreg=1;
							return 0;
						}
					default :
						{
							wrongreg=1;
							return 0;
						}
				}
			}
			else if(len==2)
			{
				switch(tokens[p].str[2])
				{
					case 'x':
						switch(tokens[p].str[1])
						{
							case 'a':
								return cpu.ax;
							case 'b':
								return cpu.bx;
							case 'c':
								return cpu.cx;
							case 'd':
								return cpu.dx;
							default :
								{
									wrongreg=1;
									return 0;
								}
						}
					case 'p':
						switch(tokens[p].str[1])
						{
							case 's':
								return cpu.sp;
							case 'b':
								return cpu.bp;
							default :
								{
									wrongreg=1;
									return 0;
								}	
						}
					case 'i':
						switch(tokens[p].str[1])
						{
							case 's':
								return cpu.si;
							case 'd':
								return cpu.di;
							default:
								{
									wrongreg=1;
									return 0;
								}
						}
					case 'h':
						switch(tokens[p].str[1])
						{
							case 'a':
								return cpu.ah;
							case 'b':
								return cpu.bh;
							case 'c':
								return cpu.ch;
							case 'd':
								return cpu.dh;
							default:
								{
									wrongreg=1;
									return 0;
								}
						}
					case 'l':
						switch(tokens[p].str[1])
						{
							case 'a':
								return cpu.al;
							case 'b':
								return cpu.bl;
							case 'c':
								return cpu.cl;
							case 'd':
								return cpu.dl;
							default:
								{
									wrongreg=1;
									return 0;
								}
						}
					default:
						{
							wrongreg=1;
							return 0;
						}
				}
			}
			else
			{
				wrongreg=1;
				return 0;
			}
		}
		else if(tokens[p].type==NUM)
		{
			int size=getsymtabsize();
			char *strtab=getstrtab();
			Elf32_Sym *symtab=getsymtab();
			int j=0;
			for(j=0;j<size;j++)
			{
				if(strcmp(tokens[p].str , strtab+symtab[j].st_name)==0) {
					return symtab[j].st_value;
				}
			}
			wrongnum=1;
			printf("Wrong identifier at position %d.",p);
			return 0; 
		}
		else 
		{
			wrongnum=1;
			printf("Wrong num at position %d.",p);
			return 0;
		}
	}	
	else if(check_parentheses(p, q) == true) {
		/* The expression is surrounded by a matched pair of parentheses. 
		 * If that is the case, just throw away the parentheses.
		 */
		return eval(e,p + 1, q - 1); 
	}
	else if(tokens[p].type==NOT)
	{
		return !eval(e,p+1,q);
	}
	else if(tokens[p].type==DEREF)
	{
		uint32_t x;
		x=eval(e,p+1,q);
		return swaddr_read(x,4,1);
	}
	else if(tokens[p].type==MINUS)
	{
		 return -eval(e,p+1,q);
	 }

	else {
		for(i=p;i<q+1;i++)
		{
			if(tokens[i].type!=NUM) 
			{	if(tokens[i].type=='(') flag++;
				if(tokens[i].type==')') flag--;
				if(flag==0&&tokens[i].type!=')'&&tokens[i].type!=NUM&&tokens[i].type!=NOT&&tokens[i].type!=DEREF&&tokens[i].type!=MINUS)
				{
					if(op==0) 
					{
						op=i;
						op_type=tokens[i].type;							}
					else if((op_type=='+'||op_type=='-')&&(tokens[i].type=='*'||tokens[i].type=='/'))  
						continue;
					else if((tokens[i].type=='+'||tokens[i].type=='-'||tokens[i].type=='*'||tokens[i].type=='/')&&(op_type==EQ||op_type==NEQ||op_type==OR||op_type==AND))
						continue;
					else
					{	
						op=i;
						op_type=tokens[i].type;
						continue;
					}	
				}
			}
		}
		val1 = eval(e,p, op - 1);
		val2 = eval(e,op + 1, q);
		switch(op_type) {
			case '+': return val1 + val2;
			case '-': return val1 -val2;
			case '*': return val1* val2;
			case '/': return val1 /val2;
			case EQ:  return val1==val2;
			case NEQ: return val1!=val2;
			case AND: return val1&&val2;
			case OR:  return val1||val2;
			default: assert(0);
			}
	}
	return n;
}
uint32_t expr(char *e, bool *success) {
	int p,q;
	uint32_t sum;
	if(make_token(e)==false) 
	{
		*success = false;
		return 0;
	}
	
	/* TODO: Insert codes to evaluate the expression. */
	p=0;q=nr_token-1;
	sum=eval(e,p,q);
	if(ilir==1)
	{
		*success=false;
		ilir=0;
		return 0;
	}
	if(wrongnum==1)
	{
		*success=false;
		wrongnum=0;
		return 0;
	}
	if(wrongreg==1)
	{
		*success=false;
		wrongreg=0;
		printf("Wrong register!\n");
		return 0;
	}
	*success=true;
	return sum;
}
