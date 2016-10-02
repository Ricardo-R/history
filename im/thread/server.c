#include "unp.h"
/*
   一个能正常经过TCP三次握手和四次分节断开的TCP/IP 并发服务器
  
   服务器listen 在指定端口等待客户端的联机, 收到客户端的联机请求服务器accept并fork一个子进程来处理
   子进程的联机。这里使用的accept和客户端使用的connect是假定在正常状态联机，因此使用的是阻塞的
   accept和connect,而不使用非阻塞的来处理由于服务器繁忙或其它原因导致的accept或connect过程中出现的
   其它问题。

   正常由客户端断开连接向服务器发送FIN分节，服务器处理子进程阻塞于read调用，收到FIN分节返回0,
   此时应处理read调用返回的0,结束调用回到服务器子进程main函数，服务器子进程终止。
*/

/*
	子进程的处理函数声明 
*/
static void *child_handle_function(void *fd);

/*
   信号处理函数,用于处理子进程返回的SIGCHLD信号，防止僵死进程产生。
*/
void sig_func(int signo)
{
	pid_t	pid;
	int	stat;
	while((pid = waitpid(-1, &stat, WNOHANG)) < 0);
	//fprintf(stdout, "child %d terminated\n", pid);
	return;
}

/*
	主函数：
		    由用户输入一个端口号指定服务器要使用的端口，
		服务器在指定端口监听，每接收到一个客户的联机就
		fork一个子进程处理
*/
main(int argc, char *argv[])
{
	int sockfd, *new_fd;
	struct sockaddr_in server_addr;
	struct sockaddr_in client_addr;
	int addrlen, portnumber, n;
	char guest_ip[INET_ADDRSTRLEN];	
	pthread_t tid;
	
	if(argc != 2)
	{
		printf("Usage: %s portnumber\a\n", argv[0]);
		exit(1);
	}
	if((portnumber = atoi(argv[1])) < 0)
	{
		printf("Usage: %s portnumber\a\n", argv[0]);
		exit(1);
	}
	
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket error: %s\a\n", strerror(errno));
		exit(1);
	}
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	server_addr.sin_port	= htons(portnumber);
	
	if(bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("Bind error: %s\n\a", strerror(errno));
		exit(1);
	}
	if(listen(sockfd, 5) == -1)
	{
		printf("Listen error: %s\a\n", strerror(errno));
		exit(1);
	}
	signal(SIGCHLD, sig_func);
	
	//循环监听等待客户端的连接
	while(1)
	{
		addrlen = sizeof(client_addr);
		if ((new_fd = (int *)malloc(sizeof(int))) == NULL)
		{
			printf ("alloc new_fd failed\n");
			exit(1);
		}
		if((*new_fd = accept(sockfd, (struct sockaddr *)(&client_addr), &addrlen)) == -1)
		{
			if(errno == ECONNABORTED)
			{//accept调用过程中由客户端发送RST引起的accept错误，ECONNABORTED是posix规范
				continue;
			}
			else
			{
				printf("accept error: %s\a\n", strerror(errno));
				exit(1);
			}
		}
		inet_ntop(AF_INET, &client_addr.sin_addr, guest_ip, sizeof(guest_ip));
		printf("Get connection from: %s\n", guest_ip);	
		//子线程处理客户的联机
		pthread_create( &tid, NULL, child_handle_function, new_fd);
	}
}

/*
   子进程处理函数
*/
static void *child_handle_function(void *arg)
{
	char sendline[BUFF], recvline[BUFF];
	int n, nbytes, stdineof, maxfd, fd;
	FILE *fp = stdin;
	fd = *(int *)arg;
	free(arg);

	pthread_detach(pthread_self());

	fd_set	rset;
	FD_ZERO(&rset);
	stdineof = 0;

	while(1)
	{
		//服务器进程阻塞于read调用，由客户端正常结束通信的tcp分节FIN到达时，read返回0
		if(stdineof == 0)
			FD_SET(fileno(fp), &rset);
		FD_SET(fd, &rset);
		maxfd = max(fileno(fp), fd) + 1;
		select(maxfd, &rset, NULL, NULL, NULL);
		
		//查看描述符是否有数据到达,有数据到达就读出并打印在标准输出
		if(FD_ISSET(fd, &rset))
		{
			bzero(recvline, sizeof(recvline));
			nbytes = read(fd, recvline, sizeof(recvline));
			if(nbytes == 0)
			{
				if(stdineof == 1)
				{
					printf("stdineof == 1\n");
					pthread_exit(NULL);
				}
				else
				{
					printf("Client terminate\n");
					pthread_exit(NULL);
				}
			}
			recvline[nbytes] = '\0';
			printf("R@ %s");
		}
		//查看标准输入是否有数据可读，有则读入并发送出去
		if(FD_ISSET(fileno(fp), &rset))
		{
			bzero(sendline, sizeof(sendline));
			n = read(fileno(fp), sendline, sizeof(sendline));
			if(n == 0)
			{
				stdineof = 1;
				printf("stdineof == 1\n");
				shutdown(fd, SHUT_WR);
				FD_CLR(fileno(fp), &rset);
				continue;
			}
			write(fd, sendline, strlen(sendline));
		}
	}
}
