#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>
#define MAXLINE 1000

int getlin(char s[], int lim);
void copy(char to[], char from[]);

main()
{
//	int nl, c, nw, nc, state;

/*	打印	
	printf("hello,%ld\n", EOF);
	printf("hello,%%\n");
	printf("hello,\c\n");
	printf("hello,\%\n");
*/
/*	EOF，统计词数
   	while(getchar() != EOF)	
		c++;
	printf("%d\n", c);
*/
/*	统计行数
	while((c=getchar()) != EOF)
		if(c == '\n')
			++nl;
	printf("%d\n", nl);
 
 
 */
/*	统计单词、函数、字符数
#define IN 1
#define OUT 0
	state = OUT;
	nl = nw = nc = 0;
	while((c=getchar()) != EOF)
	{
		++nc;
		if(c == '\n')
			++nl;
		if(c == '\n' || c == ' ' || c == '\t')
			state = OUT;
		else if(state == OUT)
		{
			state = IN;
			++nw;
		}

	}
	++nl;
	printf("\n%d %d %d\n", nc, nw, nl);
*/
/*	char *p 和 char s[];

	char *p;
	char s[100];
	size_t len=100;
	getline(&p, &len, stdin);
	getline(&s, &len, stdin);
	printf("\nP %s\n", p);
	printf("\nS %s\n", s);
*/
	int len, max;
	char line[MAXLINE], longest[MAXLINE];

	max = 0;
	while((len=getlin(line, MAXLINE)) > 0)
		if(len > max)
		{
			max = len;
			copy(longest, line);
		}
	if(max > 0)
		printf("%s\n%d\n", longest, max);
	return 0;
}

int getlin(char s[], int lim)
{
	int c, i;
	i = -1;
	while(c=getchar())	
		if(c!='\n' && ++i<lim-1 && c!=EOF)
			s[i] = c;
		else
			break;
	if(c == '\n')
		s[++i] = c;
	else
		--i;
	s[++i] = '\0';

	return --i;//返回的长度不包含'\0'和换行符
}
void copy(char to[], char from[])
{
	int c, i;
	i=-1;
	while(from[++i] != '\0')
		to[i] = from[i];
	to[i] = '\0';
}
