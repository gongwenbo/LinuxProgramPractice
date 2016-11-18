#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<unistd.h>
#include<netinet/in.h>
#include<signal.h>
#define PORT 8888
extern void sig_proccess(int signo);
extern void sig_pipe(int signo);
static int s;

void sig_proccess_client(int signo)
{
     printf("catch a exit signal\n");
     close(s);
     exit(0);
}

void process_conn_client(int s)

int main(int argc,int *argv[])
{
    struct sockaddr_in server_addr;
    int err;
    
    if(argc=1){
  	printf("pls input server addr\n");
      	return 0;
    }
    siganl(SIGINT,sig_proccess);
    siganl(SIGINT,sig_pipe);
    
    s=socket(AF_INET,SOCK_STREAM,0);
    if(s<0){
    	printf("socket error\n");
     	return -1; 
    
    }

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
    server_addr.sin_port=htons(PORT);

    inet_pton(AF_INET,argv[1],&server_addr.sin_addr);
    
    connect(s,(struct sockaddr*)&server_addr,sizeof(struct sockaddr));
    
    process_conn_client(s);
    close(s);    
       
}

void process_conn_client(int s)
{
     ssize_t size=0;
     char buffer[1024];
     
     for(;;){
     	size=read(0,buffer,1024);
        for(size>0){
	   send(s,buffer,1024,0);
	   write(1,buffer,size);
 	}
      }
}






