#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>

#define BUFF	1024


int max(int a, int b)
{
	return ((a > b) ? a : b);
}

/*
signal函数源码
  
typedef void sigfunc(int);

sigfunc *sig_handle(int signo, sigfunc *func)
{
	struct sigaction act, oact;
	act.sa_handler	= func;
	sigemptyset(&act.sa_mask);
	act.sa_flags	= 0;
	if(signo == SIGALRM)
	{
#ifdef	SA_INTERRUPT
		act.sa_flags	|= SA_INTERRUPT;	//SUN OS 4.x
#endif	
	}
	else
	{
#ifdef	SA_RESTART
		act.sa_flags	|= SA_RESTART;		//SVR4, 4.4BSD
#endif
	}
	
	if(sigaction(signo, &act, &oact) < 0)
		return(SIG_ERR);
	return(oact.sa_handler);
}
*/
