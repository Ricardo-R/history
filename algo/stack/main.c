#include "func.h"

main()
{
	double t = 2.0;
	int i=14;
	int k=i;

	printf("%2.1f \n", t);
	while(--i)
		push(t++);
	printf("\n");
	for(i=0; i<k-1; i++)
		printf("%2.1f ", pop());
	printf("\n");
}
