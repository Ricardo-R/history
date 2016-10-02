#include <stdio.h>
#include <malloc.h>
#include <stdlib.h>

//手动创建树的节点的时控制参数
#define ROOT  	0
#define LCH   	1
#define RCH   	2
#define PNULL 	9999

//创建二叉树的方式：TURE 手动创建   FALSE 自动创建
#define TURE	1
#define FALSE	0

//遍历二叉树的顺序：FIRST 先序  MIDDLE 中序 LAST 后序
#define FIRST	1	
#define MIDDLE	2
#define LAST	3 

//二叉树的存储节点
struct BiTNode 
{
	int data;
	struct BiTNode *lchild;
	struct BiTNode *rchild;
};

//创建二叉树
void create(int);

//遍历二叉树
void visit(int);

