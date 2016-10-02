#include "unp.h"

void str_cli(int sockfd, FILE *fp);

void sig_func(int signo)
{
	if(signo == SIGPIPE)
		fputs("SIGPIPE recvive\n", stdout);
	return;
}

main(int argc, char *argv[])
{
	int sockfd;
	struct sockaddr_in ser_addr;
	struct addrinfo *res;
	int portnumber;
	char a[INET_ADDRSTRLEN];
	
	if(argc != 3)
	{
		printf("Usage: %s hostname portnumber\a\n", argv[0]);
		exit(1);
	}
	
	if((portnumber = atoi(argv[2])) < 0)
	{
		printf("Usage: %s hostname portnumber\a\n", argv[0]);
		exit(1);
	}
	
	getaddrinfo(argv[1], NULL, NULL, &res);
	
	//建立sockfd
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket Error: %s\a\n", strerror(errno));
		exit(1);
	}
	
	//填充
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr	= *((struct sockaddr_in *)res->ai_addr);
	ser_addr.sin_family	= AF_INET;
	ser_addr.sin_port	= htons(portnumber);

//	调试信息
//	inet_ntop(AF_INET, &ser_addr.sin_addr, a, sizeof(a));
//	fprintf(stdout, "You ip info: %s\n", a);
	
	if(connect(sockfd, (struct sockaddr *)(&ser_addr), sizeof(ser_addr)) == -1)
	{
		printf("Connect error: %s\n", strerror(errno));
		exit(1);
	}
	
	signal(SIGPIPE, sig_func);
	//fprintf(stdout, "sockfd %d\n", sockfd);
	str_cli(sockfd, stdin);
	exit(0);
}
/*
	关于select()，linux中 0-stdin、1-stdout、2-stderr,因此其他任何的描述符都是从3开始，
	linux中的描述符是从0开始的。
	fd_set	——描述符的类型，
	
	FD_ZERO(fd_set *fdset)		—— 将fdset描述符集初始化,
	FD_SET(int fd, fd_set *fdset)	—— 将描述符fd加入到描述符集fdset中, 
	FD_CLR(int fd, fd_set *fdset)	—— 将描述符fd从描述符集fdset中移除, 
	FD_ISSET(int fd, fd_set *fdset)	—— 测试描述符集中的描述符fd是否被设置，例如描述符读就绪

	select()函数的第一个参数nfds（参见man select）,nfds参数指定待测试的描述符个数它的是最大
	的那个描述符的值加1（因为linux中的描述符是从0开始的)。
*/

/*
	read和write, 清楚读写的过程，返回值代表意义。 
 */

void str_cli(int sockfd, FILE *fp)
{
	char sendline[BUFF], recvline[BUFF];
	int nbytes, maxfd, stdineof;
	fd_set	rset;

	FD_ZERO(&rset);
	stdineof = 0;
	
	while(1)
	{
	 /*
		需要把FD_SET()和select放到循环里的原因：
			调用select函数后，只要有描述符处于就绪状态，则该描述符的相应的某些位被置1,
		    其余的描述符的所有位将会被置0。
	 */
		if(stdineof == 0)
			FD_SET(fileno(fp), &rset);	//fileno, shutdown —— include <unistd.h>
		FD_SET(sockfd, &rset);
		maxfd	= max(fileno(fp), sockfd) + 1;
		select(maxfd, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(fileno(fp), &rset))
		{
			bzero(sendline, sizeof(sendline));
			if(read(fileno(fp), sendline, sizeof(sendline)) == 0)
			{
				stdineof = 1;
				shutdown(sockfd, SHUT_WR);//注意是关闭连接套接字描述符的写半部
				FD_CLR(fileno(fp), &rset);
				continue;//跳过write
			}
			
			write(sockfd, sendline, strlen(sendline));
		}
		
		if(FD_ISSET(sockfd, &rset))
		{
			bzero(recvline, sizeof(recvline));
			nbytes = read(sockfd, recvline, sizeof(recvline));
			if(nbytes == 0)
			//收到服务器端FIN
			{	
				if(stdineof == 1)
					return;
				else
				{
					printf("Server terminate\n");
					return;
				}
			}
			recvline[nbytes] = '\0';
			printf("R@ %s", recvline);
		}
	}
}
