#include "unp.h"

main(int argc, char *argv[])
{
	int sockfd, *new_fd;
	struct sockaddr_in client_addr;
	int addrlen, portnumber, n;
	char guest_ip[INET_ADDRSTRLEN];	
	pthread_t tid;

	portnumber = PORT_CNUM;
	if ((sockfd = getaccept (&portnumber)) < 0)
	{
		printf (" getaccept error\n");
		exit (1);
	}
	printf ("listen port %d\n", portnumber);
	//listen
	if(listen(sockfd, 5) == -1)
	{
		printf("Listen error: %s\a\n", strerror(errno));
		exit(1);
	}
	while(1)
	{
		if ((new_fd = malloc(sizeof(int))) == NULL)
		{
			printf ("sockfd malloc failed\n");
			exit (1);
		}
		addrlen = sizeof(client_addr);
		if((*new_fd = accept(sockfd, (struct sockaddr *)&client_addr, &addrlen)) == -1)
		{
			//accept调用过程中由客户端发送RST引起的accept错误，ECONNABORTED是posix规范
			if(errno == ECONNABORTED)
			{
				free (new_fd);
				continue;
			}
			else
			{
				printf("accept error: %s\a\n", strerror(errno));
				exit(1);
			}
		}
		//调试信息
		inet_ntop(AF_INET, &client_addr.sin_addr, guest_ip, sizeof(guest_ip));
		printf("Get connection from: %s\n", guest_ip);	
		
		pthread_create (&tid, NULL, ser_control, new_fd);
	}
}
