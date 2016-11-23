#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>
#include<stdio.h>
#define PORT_SERV 8888
#define BUFF_LEN 256
void static udpserv_echo(int s,struct sockaddr*client)
{
	int n;
	char buff[BUFF_LEN];
	//char buff0[BUFF_LEN]="hello world";
	socklen_t len;
	while(1){
		len=sizeof(client);
		n=recvfrom(s,buff,BUFF_LEN,0,client,&len);
		printf("recieve:%s \n",buff);

		sendto(s,buff,n,0,client,len);	
	}
}

int main(int argc,char*argv[])
{
	int s;
	struct sockaddr_in addr_serv,addr_clie;
	
	s=socket(AF_INET,SOCK_DGRAM,0);
	memset(&addr_serv,0,sizeof(addr_serv));
	addr_serv.sin_family=AF_INET;
	addr_serv.sin_addr.s_addr=htonl(INADDR_ANY);
	addr_serv.sin_port=htons(PORT_SERV);
	
	bind(s,(struct sockaddr*)&addr_serv,sizeof(addr_serv));
	
	udpserv_echo(s,(struct sockaddr*)&addr_clie);

	return 0;

}
