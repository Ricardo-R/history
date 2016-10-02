/*
	c语言的结构体对齐方式验证：自身对齐、限定类型
*/

#include <stdio.h>
#include <stdlib.h>

struct sl{
	char c;
	char l;
	short k;
	int i;	
};
struct sl2{
	char c;
	short k;
	int i;
	char l;	
};
struct sl3{
	char c;
	short k;
	char sd;
	int i;
	char l;	
};

main()
{
	char 	c;
	short	s;
	int	i;
	float	f;
	double	d;
	long	l;
	struct sl *p;

	p	= (struct sl *)malloc(sizeof(struct sl));
	
	printf("c %d, s %d, i %d, f %d, d %d, l %d, ptr %d\n", sizeof(c),sizeof(s),sizeof(i),sizeof(f),sizeof(d),sizeof(l), sizeof(p));

	printf("sl ccsi %d, sl2 csic %d, sl3 cscic %d\n", sizeof(struct sl), sizeof(struct sl2), sizeof(struct sl3));
}
