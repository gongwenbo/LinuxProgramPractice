//broadcast.c an ip multicast server
#include<sys/types.h>
#include<sys/socket.h>
#include<linux/un.h>
#include<string.h>
#include<signal.h>
#include<stdio.h>
#include<error.h>
#include<unistd.h>
#include<sys/wait.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<netinet/in.h>
#include<stdlib.h>

#define MCAST_PORT 8888
#define MCAST_ADDR "224.1.1.1"

int main(void)
{
 	int s;
	struct sockaddr_in mcast_addr;
	char *buffer="test from broascast";
	s=socket(AF_INET,SOCK_DGRAM,0);
	if(s==-1){
		perror("open socket");
		exit(EXIT_FAILURE);
	}
	
	memset(&mcast_addr,0,sizeof(mcast_addr));
	mcast_addr.sin_family=AF_INET;
	mcast_addr.sin_addr.s_addr=inet_addr("224.0.0.1");
	mcast_addr.sin_port=htons(MCAST_PORT);

	while(1){
		int n=sendto(s,buffer,strlen(buffer)+1,0,(struct sockaddr *)&mcast_addr,sizeof(mcast_addr));
	if(n<0){
		perror("sendto");
		exit(EXIT_FAILURE);
	}
	sleep(2);
		
	}
	exit(EXIT_SUCCESS);
	//free();
}
