#include "ms.h"

void mergesort(int a[], int left, int right)
{	
	int i, b[sizeof(a)];
	if(left <right)
	{
		i = (left + right)/2;
		mergesort(a, left, i);
		mergesort(a, i+1, right);
		merge(a, b, left, i, right);
		copy(a, b, left, right);
	}
}

void merge(int c[], int d[], int l, int m, int r)
{
	int i = l, j = m+1, k = l;
	int q;
	
	while((i<=m) && (j<= r))
	{
		if(c[i] <= c[j])
			d[k++] = c[i++];
		else
			d[k++] = c[j++];
	}

	if(i>m)
	{
		for(q=j; q<= r; q++)
			d[k++] = c[q];
	}
	else
	{
		for(q=i; q<= m; q++)
			d[k++] = c[q];
	}
}


void copy(int a[], int b[], int left, int right)
{
	int i;

/*	printf("before sort:	");
	for(i=left; i<= right; i++)
		printf("%d ", a[i]);
	printf("\n");
*/	
	for(i=left; i<= right; i++)
		a[i] = b[i];
	
/*	printf("after sort:	");
	for(i=left; i<= right; i++)
		printf("%d ", a[i]);
	printf("\n");
*/
}
