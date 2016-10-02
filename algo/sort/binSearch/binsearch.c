#include "bs.h"

int s;
int a[]={3,4,8,10,13,15,16,17,20,28};
static int mid;
/*
	不存在的情况有三种：<最小元素， >最大元素， 在相邻的两个元素之间
*/

void search(int low, int high)
{
	mid = (low + high)/2;
	printf("search 	low %d, high %d, mid %d\n", low, high, mid);
	if(s == a[mid])
	{
		printf("Find\n");
		return;
	}
	else if(a[mid] > s)
	{
		if(low == mid)
		{
		//1. 这里不是high == low，当 high - low == 1时，即两个元素相邻
		//若此时s大于相邻的两个元素中的小的，小于相邻两个元素中的大的，
		//此时mid == low,此时比较a[mid]和s有a[mid]<s,于是在a[mid]<s条件内部
		//low = mid+1, 此时high == low, 再有mid=(low+high)/2, 其时mid==low==high,
		//此时比较a[mid]和s有a[mid]>s,则此要查找元素不存在，
		//同时也已将s和相邻两个元素比较完。
		//2. 若为<最小的元素的情况，最小时low==0, high=mid-1 == 0, mid=(low+high)/2
		//而经过比较a[mid]和s有a[mid]>s,找不到元素退出
			printf("Not found\n");
			return;
		}
		high = mid - 1;
		printf("else if	low %d, high %d, mid %d\n", low, high, mid);
		search(low, high);
	}
	else if(a[mid] < s)
	{
		if(high == low)
		{
		//在low=mid+1之前，保证>最大的元素的情况中最后一个元素被比较
		//可以与low=mid+1换位置测试最后一个元素未被比较的情况
			printf("Not found\n");
			return;
		}
		low = mid + 1;
		printf("else	low %d, high %d, mid %d\n", low, high, mid);
		search(low, high);
	}
}
