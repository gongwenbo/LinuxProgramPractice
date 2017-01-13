// tpc 多进程服务器框架
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<time.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<stdio.h>
#include<signal.h>

#define BUFFLEN 1024
#define SERVER_PORT 8888
#define BACKLOG 5
#define PIDNUME 3

static void handle_connect(int s_s)
{
	int s_c;
	struct sockaddr_in from;
	socklen_t len=sizeof(from);

	while(1)
	{
		s_c=accept(s_s,(struct sockaddr*)&from,&len);
		time_t now;
		char buff[BUFFLEN];
		int n=0;
		memset(buff,0,BUFFLEN);
		n=recv(s_c,buff,BUFFLEN,0);
		if(n>0&&!strncmp(buff,"TIME",4)){
			memset(buff,0,BUFFLEN);
			now=time(NULL);
			sprintf(buff,"%24s\r\n",ctime(&now));
			send(s_c,buff,strlen(buff),0);
		}
		
		//close client
		close(s_c);
		
	}
}

void sig_int(int num)
{
	exit(1);
}

int main(int argc,char *argv[])
{
	int s_s;
	struct sockaddr_in local;
	signal(SIGINT,sig_int);
	
	s_s=socket(AF_INET,SOCK_STREAM,0);
	memset(&local,0,sizeof(local));
	local.sin_family=AF_INET;
	local.sin_addr.s_addr=htonl(INADDR_ANY);
	local.sin_port=htons(SERVER_PORT);

	bind(s_s,(struct sockaddr*)&local,sizeof(local));
	listen(s_s,BACKLOG);
	
	//solve connection of client
	pid_t pid[PIDNUME];
	int i=0;
	for(i=0;i<PIDNUME;i++)
	{
		pid[i]=fork();
		if(pid[i]==0)
		{
			handle_connect(s_s);
		}
	}
	while(1);

	close(s_s);
	return 0;


}




























