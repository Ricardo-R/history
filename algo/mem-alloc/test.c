#include <stdio.h>
#include <stdlib.h>
#include <string.h>

main()
{
	char a[5], c, *p;
	int k, i=9;
	p = a;
	while((c=getchar())!='\n')
	{
		*p = c;
		printf("%c ", c);
		++p;
	}
	k = atoi(a);

	printf("\n%d\n", k);

}
