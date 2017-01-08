#include<stdio.h>
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

#define MAXCHILD 128
static unsigned long dest=0;
static unsigned	short dest_port=0;
static unsigned long src=0;
static int PROTO_UDP=-1;
static alive=-1;

// function of random
static inline long myrandom(int begin,int end)
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

































 
