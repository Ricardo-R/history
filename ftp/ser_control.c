#include "unp.h"

/*
   服务器处理线程：
	释放fd -> 线程脱离 -> 处理 

 */
void *ser_control(void *sockfd)
{
/*--------------------回应码信息字符串----------------------*/
	char msg_WELCOME[]	= "200 Welcome\r\n"; 
	char msg_CMDNOTFOUND[]	= "405 Command not found\r\n"; 
	char msg_GET[]	 	= "201 GET OK\r\n"; 
	char msg_GET_FAIL[]	= "404 File not found\r\n"; 
	char msg_PUT[]		= "202 PUT OK\r\n"; 
	char msg_PUT_FAIL[]	= "403 File name ilegal\r\n"; 
	char msg_PORT[]		= "204 PORT OK\r\n"; 
	char msg_CLOSE[] 	= "205 Bye Bye\r\n"; 
	char msg_CONNECT_ERR[] 	= "407 Connect error!\r\n"; 
/*--------------------回应码信息字符串----------------------*/

	int n, nbytes, fd;
	char *ptr[MAX_ARGV], str[MAX_STR];
	int ARGC, CMD, INSTR;

//	Exsh *Mtrans;
	
	Exchange ftransfer;
	char ip[INET_ADDRSTRLEN]; 
	int data_fd, portnum;
	//打开数据连接指示数据传输方向，关闭数据连接存放返回值
	int FLAG;
	socklen_t addrlen;
	struct sockaddr_in addr;
	char FileName[32];
	pthread_t tid;

	fd = *((int*)sockfd);

	free (sockfd);
	pthread_detach (pthread_self());
	
	if (writen (fd, msg_WELCOME, strlen(msg_WELCOME)) < 0)
	{
		printf (" write error: %s\n", strerror(errno));
		pthread_exit (NULL);
	}

	while(1)
	{
		//服务器进程阻塞于read调用，由客户端正常结束通信的tcp分节FIN到达时，read返回0
		nbytes = read(fd, str, sizeof(str)-1);
		if(nbytes == 0)
		{
			printf("Client terminate\n");
			pthread_exit (NULL);
		}
		//数据格式检查不通过
		if (checkData (str, nbytes) < 0)
			continue;
		str[nbytes] = '\0';
//		printf (" str=%s strlen=%d \n", str, strlen(str));
			
		//参数有客户端处理然后再发送给服务器，因此服务器不存在CMD_TOMANYARGV的错误
		CMD = split(&ARGC, ptr, str);
		
		if (CMD==CMD_MALLOCERR) 
		{
			//服务器内存不足，断开连接
			freeMalloc(ptr);
			pthread_exit (NULL);
		}

		INSTR=instruction(CON_YES, ARGC, ptr);
			
		if (INSTR == INS_CLOSE)
		{
			if (writen (fd, msg_CLOSE, strlen(msg_CLOSE)) < 0)
				printf (" write error: %s\n", strerror(errno));
			freeMalloc (ptr);
			pthread_exit (NULL);
		}
		else if (INSTR == INS_GET)
		{
			printf (" GET: %s", str);
			if (exist_file (ptr[1]) < 0)
				n = writen(fd, msg_GET_FAIL, sizeof(msg_GET_FAIL));
			else
				n = writen(fd, msg_GET, sizeof(msg_GET));
			if (n < 0)
			{
				printf (" write error: %s\n", strerror(errno));
				freeMalloc (ptr);
				pthread_exit (NULL);
			}
			FLAG 	= SEND;
			strcpy (FileName, ptr[1]);
			freeMalloc (ptr);
			continue;
		}
		else if (INSTR == INS_PUT)
		{
			printf (" PUT: %s", str);
			//检查文件名是否合法
			if (writen(fd, msg_PUT, sizeof(msg_PUT)) < 0)
			{
				printf (" write error: %s\n", strerror(errno));
				freeMalloc (ptr);
				pthread_exit (NULL);
			}
			FLAG 	= RECV;
			strcpy (FileName, ptr[1]);
			printf ("FileName : %s\n", FileName);
			freeMalloc (ptr);
			continue;
		}
		else if (INSTR == INS_PORT)
		{
			portnum = atoi (ptr[1]);
			printf ("\n PORT: %s  %d\n", str, portnum);
			if (writen(fd, msg_PORT, sizeof(msg_PORT)) < 0)
			{
				printf (" write error: %s\n", strerror(errno));
				freeMalloc(ptr);
				pthread_exit (NULL);
			}
			//从控制连接获取客户端的ip地址
			addrlen = sizeof (addr);
			getsockname (fd, (struct sockaddr *)&addr, &addrlen);
			inet_ntop (AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
			printf("Get connection from: %s\n", ip);	
			//连接到客户端
			data_fd = 0;
			if ((data_fd=getconnect (ip, portnum)) < 0)
			{
				if (writen (fd, msg_CONNECT_ERR, strlen(msg_CONNECT_ERR)) < 0)
				{
					printf (" write error: %s\n", strerror(errno));
					pthread_exit (NULL);
				}
				continue;
			}
			ftransfer.sockfd = data_fd;
			ftransfer.flag = &FLAG;
			ftransfer.FileName = FileName;
			if (FLAG == SEND)
				printf ("connect successful, send\n");
			if (FLAG == RECV)
				printf ("connect successful, RECV\n");
			pthread_create (&tid, NULL, transfer, &ftransfer);
			pthread_join (tid, NULL);
			
			//返回值FLAG处理
			handle (&FLAG);
			continue;
		}
		else
		{
			freeMalloc (ptr);
			if (writen(fd, msg_CMDNOTFOUND, sizeof(msg_CMDNOTFOUND)) < 0)
			{
				printf (" write error: %s\n", strerror(errno));
				pthread_exit (NULL);
			}
			continue;
		}
	}
}
