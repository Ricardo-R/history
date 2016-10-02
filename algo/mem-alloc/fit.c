#include "mem.h"

static points * header;//空闲块链表头节点的全局变量,使用static限定只能在本文件使用
#define MAXSIZE	140

//初始化空闲块链表
void initalize(void)
{
	points *f, *b;
	int i = 6;
		
	header = (points *)malloc(sizeof(points *));
	header->k = 6;//头节点记录空闲块的块数，头节点并不属于要分配的内存的一部分
	header->next = NULL;
	b = header;

	while(--i)
	{
		f = (points *)malloc(sizeof(points *));
		f->k = rand()%(MAXSIZE-i*10);
		f->next = NULL;
		
		b->next = f;
		b = f;
		f = f->next;
	}
	f = (points *)malloc(sizeof(points *));
	f->k = MAXSIZE;
	f->next = NULL;
	b->next = f;
}
//打印空闲链表
void print(void)
{
	points *p;
	for(p=header->next; p != NULL; p=p->next)
		printf("%d ", p->k);
	printf("\n");

}
static void getsize(int *size)
{
	char a[3], c, *p;
	int i, k;

	getchar();//读取在菜单选择时候的回车符

	while(1)
	{
		printf("请输入要申请的空间的大小，范围(0-120]:");
		//每次对a初始化是为了避免再输入超3个字符错误之后，输入少于3个字符的情况
		for(i=0;i<3;i++)
			a[i]='f';
		p = a;
		for(i=0;(c=getchar())!='\n' && i<3;i++)
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
void bassign(void)
{
	int size, far=MAXSIZE, n;
	points *ptr, *p;
	getsize(&size);
	for(p=header->next; p!=NULL; p=p->next)
		if((n=p->k-size) >= 0 && n < far)
		{	
			far = n;
			ptr = p;
		}
	printf("最佳适应法分配的空间块大小是 %d\n", ptr->k);
}
void fassign(void)
{
	int size;
	points *p;

	getsize(&size);
	for(p=header->next; p!=NULL; p=p->next)
		if((p->k-size) >= 0 )
			break;
	printf("首次适应法分配的空间块大小是 %d\n", p->k);
}

/*
此函数不能正确处理输入为字符串的情况
void getsize(int *size)
{
	char *p;
	while(1)
	{
		printf("请输入要申请的空间的大小，范围(0-120]:");
		scanf("%d", size);
		if(*size<=0 || *size>120)
			printf("输入非法！\n");
		else
			break;
	}
}
 
 */
