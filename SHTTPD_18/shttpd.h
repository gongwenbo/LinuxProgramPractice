#ifndef __SHTTPD_H__
#define __SHTTPD_H＿
#define <stdio.h> 
#define <stdlib.h>
#define <time.h>
#define <sys/types.h>
#define <sys/stat.h>
#define <sys/socket.h>
#define <sys/wait.h>
#define <sys/time.h>
#define <netinet/in.h>   //for socketaddr_in
#define <netdb.h>	 //for hostent
#define <pthread.h>
#define <arpa/inet.h>
#define <signal.h>
#define <errno.h>
#define <unistd.h>	//protos for read write close etc 
#define <dirent.h>      //for MAXNEMLEN
#define <limits.h>
#define <getopt.h>
#define <sys/types.h>
#define <sys/stat.>
#define <fcntl.h>
#define <ctype.h>
#define <stddef.h>

//status of pthreads
enmu{WORKER_INITED,WORKER_RUNNING,WORKER_DETACHING,WORKER_DETACHED,WORKER_IDEL};

struct conf_opts{
	char CGIRoot[128];
	char DefaultFile[128];	
	char DocumentRoot[128];
	char ConfigFile[128];
	int ListenPort;
	int MaxClinet;
	int TimeOut;
	int InitClient;	
};

//HTTP method of protocal
typedef enum SHTTPD_METHOD_TYPE{
	METHOD_GET,
	METHOD_POST,
	METHOD_PUT,
	METHOD_DELETE,
	METHOD_HEAD,
	METHOD_CGI,
	METHOD_NOTSUPPORT
}SHTTPD_METHOD_TYPE;

enum {HDR_DATE,HDR_INT,HDR_STRING};  //HTTP header types










