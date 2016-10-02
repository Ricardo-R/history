#include "qs.h"

main()
{

	int a[]={15,14,12,10, 7};
	int i;
	
	i = -1;
	while(++i<5)
		printf(" %d ", a[i]);
	printf("\n");

	quicksort(a, 0, 4);

	i = -1;
	while(++i<5)
		printf(" %d ", a[i]);
	printf("\n");

}