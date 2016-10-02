#include "bs.h"

extern int s;
extern int a[];

main()
{
	int low, high, i;
	
	printf("Please input a number for searching:\n");
	scanf("%d", &s);
	low	= 0;
	high	= 9;
	search(low, high);
	printf("数组a[]：");
	for(i=0; i<10; i++)
		printf("%d ", a[i]);
	printf("\n");
	exit(0);
}
