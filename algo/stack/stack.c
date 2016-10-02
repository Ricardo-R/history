#include "func.h"

static int sp=0;
static double val[MAXVAL];

void push(double s)
{
	if(sp < MAXVAL)
	{	
		printf("%2.1f ", s);
		val[sp++] = s;
	}
	else
		printf("stack is full\n");
}
double pop(void)
{
	if(--sp >= 0)
		return val[sp];
	else
	{
		printf("stack is empty\n");
		return -1;
	}
}
