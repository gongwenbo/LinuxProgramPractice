#include<sys/types.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<string.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include<unistd.h>
#include<fcntl.h>

ssize_t send_fd(int fd,void *date,size_t bytes,int sendfd)
{
	struct msghdr msgsdr_send;
	struct iovec iov[1];
	union{
		struct cmsghdr cm;
		char control[CMSG_SPACE(sizeof(int))];
	} control_un;

	struct cmsghdr*pcmsghdr=NULL;
	msgsdr_send.msg_control=control_un.control;
	msgsdr_send.msg_controllen=sizeof(control_un.control);

	pcmsghdr=CMSG_FIRSTHDR(&msgsdr_send);
	pcmsghdr->cmsg_len=CMSG_LEN(sizeof(int));
	pcmsghdr->cmsg_level=SOL_SOCKET;
	pcmsghdr->cmsg_type=SCM_RIGHTS;
	*((int*)CMSG_DATA(pcmsghdr))=sendfd;
	//*((int*)CMSG_DATA(pcmsghdr))= sendfd;
	msgsdr_send.msg_name=NULL;
	msgsdr_send.msg_namelen=0;

	iov[0].iov_base=date;
	iov[0].iov_len=bytes;
	msgsdr_send.msg_iov=iov;
	msgsdr_send.msg_iovlen=1;

	return (sendmsg(fd,&msgsdr_send,0));

}

int main(int argc,char*argv[])
{
	int fd;
	ssize_t n;
	
	if(argc!=4)
		printf("socketpair error\n");
	if((fd=open(argv[2],atoi(argv[3])))<0)
		return 0;
	if((n=send_fd(atoi(argv[1]),"",1,fd)<0))
		return 0;
	return 0;

}
