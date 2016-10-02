#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

/*
	关于指针，指针数组，++、--前缀运算符

*/

main()
{
	char *lineptr[10];
	char *line, *p;	
	FILE *fp;
	fp = fopen("test", "r");
	int  k;
	size_t n=0;

/*	int f;

	while( (k=getline(&line, &n, fp)) != -1)
	//当line为NULL时，n为编译器自动分配给line的大小，且line若不需要了需要程序员手动回收
	//当line不为NULL，n为line的大小
	{
		p = malloc(k);//p指向新申请的一块空间
		f = k;	//保存k的值，为了复制之后将p指向复制后的第一个字符的位置
		while(k--)
		{
			*p++ = *line++;
			//printf("%c", *line);
			//++p;
			//++line;
		}
		p -= f;//将p指向复制后的第一个字符的位置
		printf("# %s %d %n\n", p, k, n);//打印查看
	}
	fclose(fp);
*/
	int i = 0;

	while( (k=getline(&line, &n, fp)) != -1)
	{
		p = malloc(k);		//p指向新申请的一块空间
		while(*p++ = *line++);	//复制到k+1个字符时返回0
		p -= ++k;		//将p指向复制后的第一个字符的位置
		lineptr[i++] = p;	//lineptr是指针数组，lineptr不是变量,不能使用lineptr++等运算符
	}
	k = i;
	for(i=0; i<k; i++)
		printf("@ %s", lineptr[i]);
	
	fclose(fp);
}
