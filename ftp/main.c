#include "unp.h"

int main ()
{
	char *ptr[MAX_ARGV];
	char str[MAX_STR];
	int ARGC, CMD, INSTR, n;
	pthread_t tid;
	
	int *sockfd;

	while (1)
	{
		if (getSline(str, '>') <= 0)
			continue;
		CMD = split(&ARGC, ptr, str);
		if (CMD==CMD_TOMANYARGV || CMD==CMD_MALLOCERR) 
		{
			freeMalloc(ptr);
			continue;
		}
			
		INSTR=instruction(CON_NO, ARGC, ptr);

		if (INSTR == INS_OPEN)
		{
			//建立sockfd
			if ((sockfd = (int *)malloc(sizeof(int))) == NULL )
			{
				printf ("sockfd malloc failed\n");
				freeMalloc (ptr);
				exit (1);
			}
			if (ARGC == 2)
				*sockfd = getconnect (ptr[1], PORT_CNUM);
			else//ARGC == 3
				*sockfd = getconnect (ptr[1], atoi(ptr[2]));
			freeMalloc(ptr);

			//contrl process starts here
			if (*sockfd > 0)
			{
				pthread_create (&tid, NULL, control, sockfd);
				pthread_join (tid, NULL);
			}
			continue;
		}
		else if (INSTR == INS_EXIT)
		{
			freeMalloc(ptr);
			break;
		}
		else	//INS_OTHER
		{
			freeMalloc(ptr);
			continue;
		}
	}
}
