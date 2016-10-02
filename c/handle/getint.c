/*
	从标准输入读取用户输入的整数，处理了非法输入的情况，比scanf好用。 
 
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXSIZE 120	//允许用户输入的最大数
#define BYTES	3	//按字符的方式读入用户的输入是允许的最大整数的位数，如MAXSIZE为1120,则应是4
void getint(int *size);

main()
{
	int s;
	getint(&s);
	printf("%d\n", s);

}

void getint(int *size)
{
	char a[BYTES], c, *p;
	int i, k;

	while(1)
	{
		printf("请输入要申请的空间的大小，范围(0-120]:");
		//每次对a初始化是为了避免再输入超3个字符错误之后，输入少于3个字符的情况
		for(i=0;i<BYTES;i++)
			a[i]='f';
		p = a;
		for(i=0;(c=getchar())!='\n' && i<BYTES;i++)
			*p++ = c;
		k = i;
		for(i=0; i<k; i++)
			if(a[i] <'0' || a[i]>'9')
			{
				printf("非法输入!\n");
				break;
			}
		if(i == k)
		{
			*size = atoi(a);
			if(*size<=0 || *size>MAXSIZE)
			{
				printf("非法输入!\n");
				continue;
			}
			else
				break;
		}
	}
}
 
