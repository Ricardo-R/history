#include "unp.h"

/* NOTE
   	问题在于makestr函数构造字符串时，返回的值小于真实字符个数;
	
	服务器和客户端之间的通讯数据格式 {CMD<sp>...<sp>arg<\r\n>};
	
	if{}else if{}...else{}语句，仅执行其中最先满足条件的唯一一个，执行完后就退出来上述语句的判断
	
	使用sendline[strlen(sendline)+1]是不准确的，例：
		初值strlen(sendline) == 10
		strlen[strlen(sendline)] = '\r'  , 此时操作sendline[10] = '\r'
		strlen[strlen(sendline)+1] = '\n', 此时操作sendline[10] = '\n' 
		strlen[strlen(sendline)+1] = '\0', 此时操作sendline[11] = '\0'
	这里涉及到优先级

	单独拼接字符串一定要检查服务器端收到的和本地的大小，及与实际的大小
*/

void *control(void *sockfd)
{
	char sendline[BUFF];
	int ctrl_sockfd;
	int ARGC, CMD, RESPONSE, INSTR;
	char str[MAX_STR];
	char *ptr[MAX_ARGV];
	int n, nbytes, i;
	
	char sendport[8], FileName[32];
	//打开数据传输时作为方向标志，关闭数据传输是作为返回值设置
	int FLAG;
	struct sockaddr_in addr;
	socklen_t addrlen;
	int data_listen, data_fd, data_port;
	Exchange ftransfer;
	pthread_t tid;
	
	int optval;	//0表示禁止，非0表示启用
	socklen_t optlen;

	ctrl_sockfd = *((int*)sockfd);
	free (sockfd);
	
	optval = 1;
	optlen = sizeof(int);
	
	//禁止Nagle算法
	if (setsockopt(ctrl_sockfd, IPPROTO_TCP, TCP_NODELAY, &optval, optlen) == -1)
	{
		printf (" setsockopt failed\n");
		close (ctrl_sockfd);
		pthread_exit (NULL);
	}

	//welcome message
	response (ctrl_sockfd, sendline, READ_ONLY);

	while (1)
	{
		if (getSline(str, '>') <= 0)
			continue;
		CMD = split (&ARGC, ptr, str);
		if (CMD == CMD_TOMANYARGV || CMD == CMD_MALLOCERR)
		{
			freeMalloc (ptr);
			continue;
		}
		
		INSTR = instruction (CON_YES, ARGC, ptr);
		
		if (INSTR == INS_CLOSE)
		{	
			strcpy (sendline, "CLOSE\r\n\0");
			printf ("strlen=%d\n", strlen(sendline));
			response (ctrl_sockfd, sendline, WR_BOTH);
			close (ctrl_sockfd);
			freeMalloc (ptr);
			pthread_exit (NULL);
		}
		else if (INSTR == INS_GET)
		{
			if (ARGC < 2 || ARGC > 3)
			{	
				printf (" Usage: get R-filename [L-filename]\n");
				freeMalloc (ptr);
				continue;
			}
			//只发送命令和文件名
			n = makestr (ptr, sendline, 1);
			//printf (" strlen=%d n=%d\n", strlen(sendline), n);
			if (response (ctrl_sockfd, sendline, WR_BOTH) == START_PORT)
			{
				if (ARGC == 2)
					strcpy (FileName, ptr[1]);
				else
					strcpy (FileName, ptr[2]);
				INSTR 	= INS_PORT;
				FLAG	= RECV;
				freeMalloc (ptr);
			}
			else
			{
				freeMalloc (ptr);
				continue;
			}
		}
		else if (INSTR == INS_PUT)
		{
			if (ARGC < 2 || ARGC > 3)
			{	
				printf (" Usage: put L-filename [R-filename]\n");
				freeMalloc (ptr);
					continue;
			}
			if (exist_file (ptr[1]) < 0)
			{
				printf ("Not such file\n");
				freeMalloc (ptr);
				continue;
			}
			
			//只发送命令和文件名
			if (ARGC == 2)
				n = makestr (ptr, sendline, 1);
			else
				n = makestr (ptr, sendline, 2);
			//printf (" strlen=%d n=%d\n", strlen(sendline), n);
			if (response (ctrl_sockfd, sendline, WR_BOTH) == START_PORT)
			{
				strcpy (FileName, ptr[1]);
				INSTR 	= INS_PORT;
				FLAG	= SEND;
				freeMalloc (ptr);
			}
			else
			{
				freeMalloc (ptr);
				continue;
			}
		}
		else if (INSTR == INS_PORT)
		{
		//用户输入port指令非法
		//if{}else if{}...else{}语句，仅执行其中最先满足条件的唯一一个，执行完后就退出来上述语句块的判断
			printf ("Command NOT found\n");
			freeMalloc (ptr);
			continue;
		}
		if (INSTR == INS_PORT)
		{
			printf ("\nTEST INS_PORT\nFileName: %s %d\n", FileName, strlen(FileName));
			data_port = 0;
			if ((data_listen=getaccept (&data_port)) < 0)
			{
				printf (" getaccept error: %s\n", strerror(errno));
				continue;
			}
			if (listen (data_listen, 5) < 0)
			{
				printf("Listen error: %s\a\n", strerror(errno));
				continue;
			}
			//把端口号转为字符串
			snprintf (sendport, sizeof(sendport), "%d", data_port);
			//printf ("\nchar port: %s\n", sendport);
			strcpy (sendline, "PORT ");
			strcat (sendline, sendport);
			n = strlen(sendline);
			sendline[n++] ='\r';
			sendline[n++] ='\n';
			sendline[n] ='\0';

			if (response (ctrl_sockfd, sendline, WR_BOTH) == PORT_OK)
			{
				printf ("send port successful, start transfer\n");
				addrlen = sizeof (addr);
				if (waitready (ctrl_sockfd, data_listen) == ACCEPT_READY)
				{	
					data_fd = accept (data_listen, (struct sockaddr *)&addr, &addrlen);
					close (data_listen);
					if (data_fd < 0)
					{
						printf("accept error: %s\a\n", strerror(errno));
						continue;
					}
					ftransfer.sockfd = data_fd;
					ftransfer.flag = &FLAG;
					ftransfer.FileName = FileName;
					printf ("accept successfully, FileName %s\n", FileName);
					//开始数据传输
					pthread_create (&tid, NULL, transfer, &ftransfer);
					pthread_join (tid, NULL);
					handle (&FLAG);
					continue;
				}
				else
					response (ctrl_sockfd, sendline, READ_ONLY);
			}
			continue;
		}
		else
		{
			freeMalloc (ptr);
			continue;
		}
	}
}
