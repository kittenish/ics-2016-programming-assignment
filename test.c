#include "stdio.h"
void fun();
int main()
{
	fun();
	return 0;
}
void fun()
{
	char *p = (void *)0x8049000;
	*p = 0;
	while(*p!=0xff);
	*p = 0x33;
	*p = 0x34;
	*p = 0x86;
}
