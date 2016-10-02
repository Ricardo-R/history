#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <ctype.h>
#include <errno.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>
#include <netinet/tcp.h>
#include <sys/stat.h>
#include <fcntl.h>


/*	cmd defines
 *	MAX_ARGV	MAX arguments allowed
 *	MAX_STR		Chars of single line instructions
 */
#define ISSpace(x) isspace((int)x)
#define Isblank(x) isblank((int)x)
#define MAX_ARGV	24
#define MAX_STR		128

//split返回值
#define CMD_TOMANYARGV	1
#define CMD_MALLOCERR	-1
#define CMD_OK		0

int split(int *ARGC, char **ptr, const char *str);
//--------------------------------------------------

//instruction return value
#define INS_CLOSE	60
#define INS_OPEN	61
#define INS_OTHER	62
#define INS_EXIT	63
#define INS_GET		64
#define INS_PUT		65
#define INS_PORT	66

#define CON_NO		111
#define CON_YES		112

int instruction(int, int, char **);
//--------------------------------------------------

#define READ_ONLY	10
#define WR_BOTH		11

#define PORT_OK		20
#define START_PORT	21
#define PRINT_ONLY	22
int response (int, char *, int);

//control connection
void *control(void *);
void *ser_control(void *);

//---------------------------------------------------
/*
 	utils.c
 */
//free dynamic malloced memory when split arguments
void freeMalloc (char **);

//get single line from stdin
int getSline (char *, char );

//write n bytes
ssize_t writen (int, const void *, size_t);

//test if file exist
int exist_file (const char *);

//make string for sending
int makestr (char **, char *, int);

//检查指令格式是否符合规范
int checkData (const char *, size_t);

//connect to server
int getconnect (char *, int);

//bind and get a rand port assign by kernel
int getaccept (int *);

//使用select等待服务器回应或超时的返回值
#define W_TIMEOUT	50
#define	W_READY		51

//等待的时间
#define W_TIMESEC	10

int waittime(int, int);

#define CTRL_READY	70
#define ACCEPT_READY	71

int waitready(int , int );

//----------------------------------------------

//数据传输方向
#define SEND		30
#define RECV		31

//返回值
#define SEND_SUCC	40
#define	RECV_SUCC	42
#define OPEN_NO_PERMIT	44	
#define CREAT_NO_PERMIT	45	
#define READ_ERROR	46
#define WRITE_ERROR	47


//定义控制线程和数据线程之间传递信息的数据结构
typedef struct {
	int 	sockfd;
	int 	*flag;
	char 	*FileName;
}Exchange;

void *transfer (void *);

void handle (int *);
//--------------------------------------------------
//密码验证模块
#define AUTH_PASS	80
#define AUTH_FAIL	81

typedef struct user{
	char username[16];
	char password[16];
	struct user *next;
}PNode;

int auth (char *, char *);
//--------------------------------------------------
//port that server listening on
#define PORT_CNUM	21

#define BUFF		1024	//chat buffer size
#define FBUFF		4096	//transfer file buffer
