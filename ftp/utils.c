#include "unp.h"

/*	
 *	描述
 		将用户一行输入分解为多个参数，实现变长参数处理
 	
 *	参数
		ARGC	处理完成把处理后得到的参数个数的值出入ARGC
		ptr	指向char *型的指针，保存处理之后的各个参数，参数个数由ARGC指出
		str	指向要处理的用户输入，必须以'\n'或'\r\n'结尾

 *	返回值
		以下宏在头文件unp.h中定义
	
		CMD_MALLOCERR	内存空间不足，分配内存失败
		CMD_TOMANYARGV	用户输入的参数超过最大限制，最大限制在unp.h中定义为宏MAX_ARGV
		CMD_OK		处理成功	
		
 */
int split(int *ARGC, char **ptr, const char *str)
{
	char *p;
	int i, k, n, argc;

	argc=0;
	
	//忽略第一个参数前面的空格或tab（若有）
	for (i=0; Isblank(str[i])!=0 && i<MAX_STR; ++i);
	while (1)
	{
		//参数处理完成。同时处理输入只有空白符加换行符的情况，及结尾是空白符加换行符的情况
		if (str[i] == '\n')
				break;
		if (str[i] == '\r' && str[i+1] == '\n')
				break;
		//计算当前参数的长度
		for (k=i; ISSpace(str[i])==0; ++i);
		n = i - k;
		if ((p = (char *)malloc((n+1)*sizeof(char))) == NULL)
			return CMD_MALLOCERR;
		strncpy(p, &str[k], n);
		ptr[argc++] = p;
		//每个串都是以'\0'结尾
		p += n;
		*p = '\0';
		if(argc >= MAX_ARGV-1)
			return CMD_TOMANYARGV;
		//忽略两个参数之间的一个或多个空格或tab
		for (; Isblank(str[i])!=0; ++i);
	}
	//保证ptr是以NULL指针作为参数指针的结尾
	ptr[argc] = NULL;
	*ARGC = argc;
	return CMD_OK;
}
/*
	描述	
   		识别并处理用户输入的指令并返回相应的整型值

	参数
		flag	当前是否处于控制连接状态，在头文件unp.h中定义宏CON_NO、CON_YES指示当前状态
		argc	指示ptr中的参数的个数
		ptr	参数列表
	
	返回值
		以下宏在头文件unp.h中定义

		INS_OPEN	打开一个到服务器的控制连接
		INS_GET		从服务器下载文件
		INS_PUT		向服务器上载文件
		INS_CLOSE	关闭控制连接
		INS_OTHER	其它不需要做任何处理的指令
		INS_EXIT	退出
*/

int instruction(int flag, int argc, char **ptr)
{
	//No instruct 
	if (argc == 0)
		return INS_OTHER;
	if (strcasecmp(ptr[0], "open") == 0)
	{
		if (flag == CON_NO)
		{
			if (argc != 2 && argc != 3)
			{
				printf (" Usage: open server-ipaddr [port]\n");
				return INS_OTHER;
			}
			else 
				return INS_OPEN;
		}
		else
		{
			printf (" Must disconnect first\n");
			return INS_OTHER;
		}
	}
	else if (strcasecmp(ptr[0], "exit")==0)
	{
		if (flag == CON_NO)
			return INS_EXIT;
		else
		{
			printf (" Connected now, use close to break connection\n");
			return INS_OTHER;
		}
	}
	else if (strcasecmp(ptr[0], "close")==0)
	{
		if (flag == CON_YES)
		{
			return INS_CLOSE;
		}
		else
		{	
			printf (" Not connect, use exit to quit\n");
			return INS_OTHER;
		}
	}
	else if (strcasecmp(ptr[0], "get") == 0)
	{
		if (flag == CON_YES)
		{
			return INS_GET;
		}
		else
		{
			printf (" Not connect \n");
			return INS_OTHER;
		}
	}	
	else if (strcasecmp(ptr[0], "put") == 0)
	{
		if (flag == CON_YES)
		{
			return INS_PUT;
		}
		else
		{
			printf (" Not connect \n");
			return INS_OTHER;
		}
	}	
	else if (strcasecmp(ptr[0], "port") == 0)
	{
			return INS_PORT;
	}
	else if (strcasecmp(ptr[0], "?") == 0)
	{
		printf (" open exit close get put\n");
		return INS_OTHER;
	}	
	else
	{
		printf (" Command NOT found\n");
		return INS_OTHER;
	}
}

/*
   	描述
   		释放处理参数动态分配的内存
  	
	参数
		p	指向要释放的参数列表
	
	返回值
		无返回值
 
 */
void freeMalloc (char **p)
{
	int i;
	char *temp;

	for (i=0; i<MAX_ARGV; i++)
	{
		if (p[i] == NULL)
			return;
		else
		{	
			temp = p[i];
			free(temp);
		}
	}
}
/*
 	描述
		从标准输入获取一行输入，并且以'\n'作为字符串的结束
	
	参数
		dst	指向输入缓冲区

	返回值
		
		输入全为空格或是tab，没有字符时，返回-1，否则返回从标准输入获取的字符的个数，
		包括最后的换行符（'\n'）在内但不包括null-teminate('\0')
		
 */

int getSline (char *dst, char s)
{
	char c;
	int i, k;
	
	printf (" %c ", s);
	fflush (stdin);

	for (i=0; c = getchar (); ++i)
	{
		dst[i] = c;
		if (c == '\n')
			break;//The loop's condition '++i' will not be executed when jmp out
	}
	k = i;//i starts from 0
	for (i=0; isblank(dst[i])!=0 && i<MAX_STR; ++i);
	if (k==i)
		return (-1);
	return (k-i+1);
}

/*	
	描述
		将src指向的字符串开始的n个字节写入到描述符fd
	
	参数
		fd	写入的描述符
		src	源字符串
		n	写入的字节数

	返回值
		成功返回写入的字节个数，失败返回-1
 
 */
ssize_t writen (int fd, const void *src, size_t n)
{
	size_t nleft;
	size_t nwritten;
	const char *ptr;

	ptr = src;
	nleft = n;
	while (nleft > 0)
	{
		if ((nwritten=write(fd, ptr, nleft)) == 0)
		{
			if (nwritten < 0 && errno == EINTR)
				nwritten = 0;
			else
				return (-1);
		}
		nleft -= nwritten;
		ptr += nwritten;
	}
	return (n);
}

/*
 	描述
		测试文件是否存在

	参数
		filename	测试的文件名

	返回值
		成功返回0,失败返回-1
 
 */
int exist_file (const char *filename)
{
	FILE *fp;
	if ((fp=fopen (filename, "r")) == NULL)
		return (-1);
	else
	{
		fclose (fp);
		return 0;
	}
}

/*
	描述
		把ptr指向的前2个参数连接起来，以空格符作为分隔符，最后以'\r\n\0'作为结束符，
		最后结束的'\0'是必须的，是防止使用strlen时出错
	
	参数
		ptr		参数列表
		dest		目的串保存地址
		x		指示ptr中参数的下标，从0开始

	返回值
		返回拼接好的串的长度n，长度n不包含'\0'在内

*/
int makestr (char **ptr, char *dest, int x)
{
	int n;	
	n = strlen(ptr[0]);
	strncpy (dest, ptr[0], n);
	dest[n++] = ' ';
	strncpy (&dest[n], ptr[x], strlen(ptr[x]));
	n +=  strlen(ptr[x]);
	dest[n++] = '\r';
	dest[n++] = '\n';
	dest[n+1] = '\0';
	return n;//返回长度并不包含'\0'
}

/*
 	描述
		使用select测试描述符在sec时间内是否准备好被读取

	参数
		sockfd	测试的描述符
		sec	设置超时时间

	返回值
		以下宏在头文件unp.h中定义
		
		W_TIMEOUT	超时返回
		W_READY		描述准备好被读取，有数据到达或是发生错误
 
 
 */
int waittime(int sockfd, int sec)
{
	struct timeval waitsec;
	fd_set	rset;
	int ready;

	waitsec.tv_sec	= sec;
	waitsec.tv_usec	= 0;
	ready = 0;

	FD_ZERO(&rset);
	FD_SET(sockfd, &rset);
	ready = select(sockfd+1, &rset, NULL, NULL, &waitsec);//这里要设置时间
		
	if(ready == 0)
		return W_TIMEOUT;
	return W_READY;
}

/*
 	描述
		getconnect 函数使用connect连接到ip的port端口

	参数
		ip	指向点分的十进制ip地址
		port	若不为0则使用该端口，否则由系统自动分配端口号，然后把系统分配的端口号保存在port指向的地址

	返回值
		成功返回连接好的连接描述符，失败返回-1


 */
int getconnect (char *ip, int port)
{
	int sockfd;
	struct sockaddr_in ser_addr;

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket Error: %s\n", strerror(errno));
		return (-1);
	}
	//填充
	bzero(&ser_addr, sizeof(ser_addr));
	if (inet_pton(AF_INET, ip, &ser_addr.sin_addr) == 0)
	{
		printf (" Ip address ilegal\n");
		return (-1);
	}
	ser_addr.sin_family	= AF_INET;
	ser_addr.sin_port	= htons(port);
	if(connect(sockfd, (struct sockaddr *)(&ser_addr), sizeof(ser_addr)) == -1)
	{
		printf (" Connect error: %s\n", strerror(errno));
		return (-1);
	}
	
	return sockfd;
}

/*
 	描述
		在本地主机上创建一个bind在由内核随机分配的端口上的TCP套接字
	
	参数
		port	若大于0则bind到该端口，否则存放内核自动分配的端口号	
	
	返回值
		成功返回bind的描述符，失败返回-1
	
 */
int getaccept (int *port)
{
	int sockfd;
	struct sockaddr_in server_addr;
	struct sockaddr_in addr;
	socklen_t addrlen;

	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
	{
		printf("Socket error: %s\a\n", strerror(errno));
		return (-1);
	}
	//server填充sockaddr结构
	bzero(&server_addr, sizeof(struct sockaddr_in));
	server_addr.sin_family = AF_INET;
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	if (*port > 0)
		server_addr.sin_port	= htons(*port);
	else
		server_addr.sin_port	= htons(0);
	//bind sockfd descritor
	if(bind(sockfd, (struct sockaddr *)(&server_addr), sizeof(struct sockaddr)) == -1)
	{
		printf("Bind error: %s\n\a", strerror(errno));
		return (-1);
	}
	addrlen = sizeof (struct sockaddr);
	if (getsockname (sockfd, (struct sockaddr *)&addr, &addrlen) < 0)
	{
		printf("getsockname error: %s\n\a", strerror(errno));
		return (-1);
	}
	*port = ntohs(addr.sin_port);
	return sockfd;
}

/*
 	描述
		处理服务器的回应信息
	
	参数
		sockfd		控制连接描述符
		ptr		split使用的指针数组
		sendmsg		要发送的命令和参数
		flag		是否只读等待服务器的信息
	
	返回值
		PRINT_ONLY	调用函数不做任何相关处理工作
		START_PORT	具备传输数据条件，可以开始协商端口
		PORT_OK		端口协商成功
	
	flag的取值READ_ONLY和WR_BOTH， PORT_OK， START_PORT， PRINT_ONLY均在头文件unp.h中定义
 */
int response (int sockfd, char *sendmsg, int flag)
{	
	int nbytes, argc, i;
	char str[MAX_STR], *ptr[MAX_ARGV];

	if (flag == READ_ONLY)
		goto read_welcome;
	
	if (writen(sockfd, sendmsg, strlen(sendmsg)) < 0)
	{
		printf (" write error: %s, exit\n", strerror(errno));
		close (sockfd);
		pthread_exit (NULL);
	}

read_welcome:
	if (waittime (sockfd, W_TIMESEC) == W_READY)
	{
		if ((nbytes=read(sockfd, str, sizeof(str)-1)) < 0)
		{
			printf (" read error: %s, exit\n", strerror(errno));
			close (sockfd);
			pthread_exit (NULL);
		}
		str[nbytes] = '\0';
		if (nbytes == 0)
		{
			printf (" server process terminate unexpected\n");
			close (sockfd);
			pthread_exit (NULL);
		}
		else
		{//回应处理
			split (&argc, ptr, str);
			//处理回应 A、打印输出即可 B、需要做后续操作
			if (strcmp (ptr[0], "201") == 0 || strcmp (ptr[0], "202") == 0)
			{
				freeMalloc (ptr);
				return START_PORT;
			}
			else if (strcmp (ptr[0], "204") == 0 )
			{
				freeMalloc (ptr);
				return PORT_OK;
			}
			else
			{
			 //	200, 405, 404, 403, 205, 407
			  
				for (i=1; i<argc; i++)
					printf (" %s", ptr[i]);
				printf ("\n");
				freeMalloc (ptr);
				return PRINT_ONLY;
			}
		}
	}
	printf (" Timeout, no answer from server\n");	
	return PRINT_ONLY;
}

/*
 	描述
		返回描述符中最先准备好的一个
	参数
		c_fd	控制连接描述符
		a_fd	监听描述符
	返回值
		CTRL_READY	控制连接描述符准备好
		ACCEPT_READY	监听描述符准备好
 */

int waitready(int c_fd, int a_fd)
{
	int maxfd;
	fd_set	rset;

	FD_ZERO(&rset);
		
	while(1)
	{
		FD_SET(c_fd, &rset);
		FD_SET(a_fd, &rset);
		maxfd	= (c_fd > a_fd ? c_fd : a_fd) + 1;
		select(maxfd, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(c_fd, &rset))
			return CTRL_READY;

		if(FD_ISSET(a_fd, &rset))
			return ACCEPT_READY;
	}
}

/*	
	描述
		处理数据传输线程的返回值并显示相应信息

	参数
		ptr	指向返回值的指针
	
	返回值
		无返回值
 */
void handle (int *ptr)
{
	if (*ptr == SEND_SUCC)
	{
		printf ("Send successful\n");
		return;
	}
	else if (*ptr == RECV_SUCC)
	{
		printf ("Recv successful\n");
		return;
	}
	else if (*ptr == OPEN_NO_PERMIT)
	{
		printf ("Open fail\n");
		return;
	}
	else if (*ptr == CREAT_NO_PERMIT)
	{
		printf ("Permission denied\n");
		return;
	}
	else if (*ptr == READ_ERROR)
	{
		printf ("Socket read error\n");
		return;
	}
	else if (*ptr == WRITE_ERROR)
	{
		printf ("Socket write error\n");
		return;
	}
	else
	{
		printf ("No support\n");
		return;
	}
}

/*
   	描述
   		验证用户名和密码，用户名密码存放在文件格式:username<sp>password<\n>
	
	参数
		user	验证的用户名
		pass	验证的密码

	返回值
		验证通过返回AUTH_PASS， 验证失败返回AUTH_FAIL

 */

int auth (char *user, char *pass)
{	
	char **ptr;//这样仅仅声明，并未分配空间
	char *seperate[3];
	FILE *fp;
	int n, argc, nbytes, FLAG;
	PNode *head, *p, *temp;
	
	ptr = (char **)malloc(sizeof(char **));
	n = 32;//用户名和密码最大长度
	if ((*ptr = (char *)malloc(sizeof(char)*n)) == NULL)
	{
		printf (" Password Malloc fail\n");
		exit (1);	
	}
	
	//密码文件中不能存在空行
	fp = fopen ("pass", "r");
	
	if ((temp = (PNode *)malloc(sizeof(PNode))) == NULL)
	{
		printf (" PNode Malloc fail\n");
		exit (1);	
	}
	temp->next = NULL;
	
	nbytes = getline (ptr, (size_t *)&n, fp); 
	(*ptr)[nbytes] = '\n';
	split (&argc, seperate, (*ptr));
	strcpy (temp->username, seperate[0]);
	strcpy (temp->password, seperate[1]);
	freeMalloc (seperate);	
	
	//printf ("%s %s", temp->username, temp->password);

	head	= temp;
	p	= head;

	//构造用户名密码链表	
	while ((nbytes=getline (ptr, (size_t *)&n, fp)) > 0)
	{
		if ((temp = (PNode *)malloc(sizeof(PNode))) == NULL)
		{
			printf (" PNode Malloc fail\n");
			exit (1);	
		}
		temp->next = NULL;
		
		(*ptr)[nbytes] = '\n';
		split (&argc, seperate, (*ptr));
		strcpy (temp->username, seperate[0]);
		strcpy (temp->password, seperate[1]);
		freeMalloc (seperate);	
		p->next = temp;
		p = p->next;
		temp = NULL;
	}
	fclose (fp);
	//遍历比较用户名密码链表
	p = head;
	while (p->next != NULL)
	{
		if (strcmp (user, p->username) == 0)
		{
			//printf ("Find user\n");
			if (strcmp (pass, p->password) == 0)
			{
				//printf ("Authenticated PASS\n");
				FLAG = AUTH_PASS;
				break;
			}
			FLAG = AUTH_FAIL;
			break;
		}
		p = p->next;
	}
	//释放内存
	temp = head;
	while (temp != NULL)
	{
		p = temp;
		temp = temp->next;
		free (p);
	}
	
	if (FLAG == AUTH_PASS)
		return AUTH_PASS;
	
	return AUTH_FAIL;
}

/*
 	描述
		检查接收到的指令格式是否符合要求，符合要求的指令的最后一个字节必须是'\n'，且中间部分没有'\n'
	
	参数
		src指向要检查的字符串的地址，len的值为src指向的串的长度（不包含结束符'\0'在内），
		若len为0则使用默认的最大缓冲空间大小MAX_STR，MAX_STR在头文件unp.h中定义

	返回值
		指令符合规范返回1，否则返回-1
 
 
 
 */
int checkData (const char *src, size_t len)
{
	int i;
	if (len == 0)
	{
		len = MAX_STR;
		for (i=0; i<len; ++i)
		{
			if (src[i] == '\n')
				return 1;
		}
		return (-1);
	}
	else
	{
		for (i=0; i<len; ++i)
		{
			if (src[i] == '\n')
				if (i == len-1)
				{
					printf (" i=%d\n", i);
					return 1;	//末尾是以'\n'结尾
				}
				else
				{
					printf (" i=%d\n", i);
					return (-1);	//指令格式中间含'\n'
				}
		}
		printf (" i=%d\n", i);
		return (-1);
	}
}
