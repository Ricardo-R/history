#include "unp.h"

void client_connected(int sockfd, FILE *fp);

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
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket Error: %s\a\n", strerror(errno));
		exit(1);
	}
	
	bzero(&ser_addr, sizeof(ser_addr));
	ser_addr	= *((struct sockaddr_in *)res->ai_addr);
	ser_addr.sin_family	= AF_INET;
	ser_addr.sin_port	= htons(portnumber);

/*
	调试信息
	inet_ntop(AF_INET, &ser_addr.sin_addr, a, sizeof(a));
	fprintf(stdout, "You ip info: %s\n", a);

*/
	if(connect(sockfd, (struct sockaddr *)(&ser_addr), sizeof(ser_addr)) == -1)
	{
		printf("Connect error: %s\n", strerror(errno));
		exit(1);
	}
	
	client_connected(sockfd, stdin);
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


void client_connected(int sockfd, FILE *fp)
{
	char sendline[BUFF], recvline[BUFF];
	int nbytes, maxfd, stdineof;
	fd_set	rset;

	FD_ZERO(&rset);
	stdineof = 0;

	/*
	 这里实现的是使用select的阻塞式读写，如果向服务器发送大量的数据导致发送缓冲区填满，那么此时
	 调用write就会阻塞，若同时又有大量数据到达，因为阻塞在write调用，导致无法读，此时接受缓冲空间
	 会被占满，后继到达的服务器的数据就会被丢弃。
	 解决这个问题可以使用多线程，那么即使write阻塞了也不会影响读取服务器端发来的信息。

	*/

	while(1)
	{
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
				/*
				 客户输入EOF主动关闭，设置标志stdineof，关闭TCP连接的写半部，发送FIN
				 保持TCP连接的读半部打开，以期服务器端发送但可能未到达的数据及服务器端的FIN
				*/
				stdineof = 1;
				shutdown(sockfd, SHUT_WR);//关闭连接套接字描述符的写半部
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			
			write(sockfd, sendline, strlen(sendline));
		}
		
		if(FD_ISSET(sockfd, &rset))
		{
			bzero(recvline, sizeof(recvline));
			nbytes = read(sockfd, recvline, sizeof(recvline));
			if(nbytes == 0)
			{	/*
				 收到服务器端的FIN分节，且是本端执行的主动关闭，返回主函数-退出。
				 若非本端执行主动关闭，则收到的是服务器端的FIN或是RST分节，表明
				 服务器端进程崩溃，打印是服务器执行关闭连接的提示，返回主函数-退出。
				*/
				if(stdineof == 1)
					return;
				else
				{
					printf("Server terminate\n");
					return;
				}
			}
			/*
			 正常接收到服务器端发过来的数据打印输出到标准输出 
			*/
			recvline[nbytes] = 0;
			printf("R@ %s", recvline);
		}
	}
}
