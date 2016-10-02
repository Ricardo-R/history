/*
	创建链表
*/

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
typedef struct ChainNode
{
	int data;
	struct ChainNode *next;
}single;

single *create(single *head)
{
	single *p1, *p2;
	p1=(single *)malloc(sizeof(single *));
	head=p1;
	scanf("%d", &p1->data);
	
	int i;
	for(i=0; i<5; i++)
	{
		p2=(single *)malloc(sizeof(single *));
		if(p2==NULL){
			printf("Malloc fail!\n");
			exit(1);
		}
		scanf("%d", &p2->data);
		
		p2->next=NULL;
		p1->next=p2;
		p1=p2;
	}		
	return head;
}

int main()
{
	single *head, *p;
	head=NULL;
	head=create(head);
	while(head)
	{
		printf("%d\t", head->data);
		p=head;
		head=head->next;
		free(p);
	}
	printf("\n");
	return 0;
}
