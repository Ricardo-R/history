#include "fib.h"

main()
{
	int a, b, s;
	printf("请输入：");
	scanf("%d", &s);
	a = recuf(s);
	b = iterf(s);
	printf("a %d, b %d\n", a, b);
}
