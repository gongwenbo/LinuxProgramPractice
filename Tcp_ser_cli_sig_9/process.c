#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/uio.h>

static struct iovec*vs=NULL,*vc=NULL;
void process_conn_server(int s)
{
	char buffer[30];
	ssize_t size=0;

	struct iovec*v=(struct iovec*)malloc(3*sizeof(struct iovec));
	
	if(!v){
	 	printf("not enougth memory!");
		return ;
	}

	vs=v;

	v[0].iov_base=buffer;
	v[1].iov_base=buffer+10;
	v[2].iov_base=buffer+20;
	v[0].iov_len=v[1].iov_len=v[2].iov_len=10;

	for(;;){
		size=readv(s,v,3);
		if(size==0){
			return ;
		}

		sprintf(v[0].iov_base,"%zu",size);
		sprintf(v[1].iov_base,"bytes");
		sprintf(v[2].iov_base,"altogether!");

	    v[0].iov_len=strlen(v[0].iov_base);
		v[1].iov_len=strlen(v[1].iov_base);
		v[2].iov_len=strlen(v[2].iov_base);
		writev(s,v,3);
	}
}


void process_conn_client(int s)
{
	char buffer[30];
	ssize_t size=0;
	int i=0;

	struct iovec*v=(struct iovec*)malloc(3*sizeof(struct iovec));
 	if(!v){
          printf("not enougth memory!");
          return ;
    }

	vc=v;
  	v[0].iov_base=buffer;
    v[1].iov_base=buffer+10;
    v[2].iov_base=buffer+20;
    v[0].iov_len=v[1].iov_len=v[2].iov_len=10;
	
	for(;;){
		size=read(0,v[0].iov_base,10);
		if(size>0){
			v[0].iov_len=size;
			writev(s,v,1);
			v[0].iov_len=v[1].iov_len=v[2].iov_len=10;
			size=readv(s,v,3);
			for(i=0;i<3;i++){
				write(1,v[i].iov_base,v[i].iov_len);
			}
		}
	}	
}


void sig_proccess(int signo)
{
	printf("catch a exit signal\n");  //release resource 
	free(vs);
	free(vc);
	_exit(0);
}

void sig_pipe(int sign)
{
	printf("catch a SIGPIPE signal\n");

	free(vc);
	free(vs);
	_exit(0);
}





























