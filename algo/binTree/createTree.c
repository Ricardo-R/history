#include "bt.h"

static struct BiTNode *root;

//处理输入的整数，确保所有输入均被正确处理
static void getint(int *size, int BYTES, int MAXSIZE)
{
	char a[BYTES], c, *p;
	int i, k;

	while(1)
	{
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
			if(*size<0 || *size>MAXSIZE)
			{
				printf("非法输入!\n");
				continue;
			}
			else
				break;
		}
	}
}

//先序递归创建二叉树
static struct BiTNode *Rcreate(int n)
{
	struct BiTNode *p;
	if(!(p=(struct BiTNode *)malloc(sizeof(struct BiTNode *))))
		return p;
	p->data = n;
	--n;
	if(n)
	{
		p->lchild=Rcreate(n);
		p->rchild=Rcreate(n);
	}
	else
	{
		p->lchild=NULL;
		p->rchild=NULL;
	}
	return p;
}

//先序递归创建二叉树
static struct BiTNode *Hcreate(int flag, struct BiTNode *pre)
{
	struct BiTNode *p;
	if(!(p=(struct BiTNode *)malloc(sizeof(struct BiTNode *))))
		return p;
	if(flag==ROOT)
	{
		printf("请输入根节点的值:");
		getchar();
	}
	else if(flag==LCH)
		printf("请输入 %d 的左孩子,为空请输入%d:\n", pre->data, PNULL);
	else	
		printf("请输入 %d 的右孩子,为空请输入%d:\n", pre->data, PNULL);

	//scanf("%d", &p->data);//scanf无法处理非法输入的情况
	getint(&p->data, 4, 9999);

	if(p->data==PNULL)
		return  NULL;
	else
	{
		p->lchild=Hcreate(LCH, p);
		p->rchild=Hcreate(RCH, p);
		return p;
	}

}

//先序遍历
static void Fvisit(struct BiTNode * p)
{
	if(p==NULL)
		return;
	printf("%d ", p->data);
	Fvisit(p->lchild);
	Fvisit(p->rchild);
}

//中序遍历
static void Mvisit(struct BiTNode * p)
{
	if(p==NULL)
		return;
	Mvisit(p->lchild);
	printf("%d ", p->data);
	Mvisit(p->rchild);
}

//后序遍历
static void Lvisit(struct BiTNode * p)
{
	if(p==NULL)
		return;
	Lvisit(p->lchild);
	Lvisit(p->rchild);
	printf("%d ", p->data);
}

void visit(int sign)
{
	if(sign == FIRST)
	{
		printf("先序遍历二叉树：");
		Fvisit(root);
		printf("\n");
	}
	if(sign == MIDDLE)
	{
		printf("中序遍历二叉树：");
		Mvisit(root);
		printf("\n");
	}
	if(sign == LAST)
	{
		printf("后序遍历二叉树：");
		Lvisit(root);
		printf("\n");
	}
}

void create(int sign)
{
	int s;
	
	if(sign)//有提示创建二叉树
		root = Hcreate(ROOT, root);
	if(!sign)
	{
		printf("请输入要创建的二叉树的层数：");
		//必不可少，否则会受到printf之后的控制字符的影响
		getchar();
		getint(&s, 2, 99);
		root = Rcreate(s);
	}
	if(root)
		return;

	printf("创建二叉树失败！\n");
}
