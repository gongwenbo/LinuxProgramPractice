#define IP_FOUND "IP_FUND"
#define IP_FOUND_ACK "IP_FOUND_ACK"

void HandleIPFound(void*arg)
{
	#define BUFFER_LEN 32;
	int ret=-1;
	SOCKET sock=-1;
	struct sockaddr_in local_addr;
	struct sockaddr_in form_addr;
	int from_len;
	int count=-1;
	fd_set readfd;
	char buff[BUFFER_LEN];
	struct timeval timeout;
	timeout.tv_sec=2;
	timeout.tv_usec=0;
	
	DEGPRINT("==>heandleIPFound");   //DEGPRINT只是代码表示，不是库函数

	sock=socket(AF_INET,SOCK_DGRAM,0);
	if(sock<0){
		DEGPRINT("handleIPFounud:socket int error\n");
		return ;
	}

	memset((void*)&local_addr,0,sizeof(struct sockaddr_in));
	
	local_addr.sin_family=AF_INET;
	local_addr.sin_addr.s_addr=htonl(INADDE_ANY);
	local_addr.sin_port=htons(MCAST_PORT);
	
	ret=bind(sock,(struct sockaddr*)&local_addr,sizeof(local_addr));
	if(ret!=0){
		DEGPRINT("handIPFound:bound error\n");
		return ;
	}

	
	while(1)
	{
		FD_ZORO(&readfd);
		FD_SET(sock,&readfd);
		ret=selecsocket(sock+1,&readfd,NULL,NULL,&timeout);
		switch(ret)
		{
			case -1:
				break;
			case 0:
				break;
			default:
				if(FD_ISSET(sock,&readfd)){
					count=recvform(sock,buff,BUFFER_LEN,0,(struct sockaddr*)&form_addr,&from_len);
					DEGPRINT("recvmsg is %s\n",buff);
					if(strstr(buff,IP_FOUND)){
						memcpy(buff,IP_FOUNUD_ACK,strlen(IP_FOUNUD_ACK)+1);
						count=sendto(sock,buff,strlen(buff),0,(struct sockaddr*)&form_addr,&form_len);
	
					}
				}
		}
	}

	PRINT("<==HandIPFound\n");
	return;

}
