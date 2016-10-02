#include "ms.h"

main()
{
	int a[]={2,8,4,10,9,6,12};
	int i;
	
	printf("before sort:	");//一个整型占四个字节
	for(i=0; i<= sizeof(a)/4; i++)
		printf("%d ", a[i]);
	printf("\n");
	
	mergesort(a, 0, 7);
	
	printf("after sort:	");
	for(i=0; i<= sizeof(a)/4; i++)
		printf("%d ", a[i]);
	printf("\n");
}