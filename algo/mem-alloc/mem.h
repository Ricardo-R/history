#include <stdio.h>
#include <stdlib.h>

//模拟空闲链表节点的空闲空间大小
typedef struct po{
	int k;
	struct po *next;
}points;

void initalize(void);
void print(void);
void fassign(void);
void bassign(void);
