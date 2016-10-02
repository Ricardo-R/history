#include "qs.h"

void swap(int *a, int *b)
{
	int temp = *a;
	*a = *b;
	*b = temp;
}
void quicksort(int a[], int p, int r)
{
	if(p<r)
	{
		int q;
		q = parti(a, p, r);
		quicksort(a, p, q-1);
		quicksort(a, q+1, r);
	}
}

int parti(int a[], int p, int r)
{
	int i = p, j = r+1;
	int x = a[p];

	while(1)
	{
		while(a[++i]<x && i<r);
		while(a[--j]>x);
		if(i>=j)
			break;
		swap(&a[i], &a[i]);
	}
	a[p] = a[j];
	a[j] = x;
	return j;
	
}
