/*
	本程序实现输入参数处理的功能,可以处理单独的参数或是合并的参数	
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char **argv)
{
	int c;

	while(--argc > 0 && **++argv == '-')//*++p内容为 char *类型的指针，指向当前这个参数的字符串的位置
	{	
		c = strlen(*argv);
		printf("%d\n", c);
		while(--c)
			switch(*++*argv)
			{
				case 'd':
					printf("d\n");
					break;
				case 't':
					printf("t\n");
					break;
				case 'f':
					printf("f\n");
					break;
				default:
					printf("没有这个选项\n");
			}
	}

}
