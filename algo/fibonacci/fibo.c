#include "fib.h"

int recuf(int n)
{//递归实现
	if(n <= 1)
		return 1;
	return recuf(n-1)+recuf(n-2); 
}

int iterf(int t)
{//迭代实现
	int f1 = 1, f2 = 1 ,i;
	
	for(i=0; i<t/2; i++)
	{
		f1=f1+f2;
		f2=f1+f2;
	}
	if(t%2 == 1)
		return f2;
	return f1;
}
