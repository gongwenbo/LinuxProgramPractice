#include "shttpd.h"

int GenerateDirFile(struct worker_ctl *wctl)
{
	struct conn_request *req=&wctl->conn.con_req;
	struct conn_response *res=&wctl->conn.con_res;
	char *command=strstr(req->uri,CGISTR)+strlen(CGISTR);
	char *arg[ARGNUM];
	int num=0;
	char *rpath=wctl->conn.con_req.rpath;
	stat *fs=&wctl->conn.con_res.fsate;
	
	DIR *dir=opendir(rpath);
	if(dir==NULL){
		res->status=500;
		retval=-1;
		goto EXITgenerateIndex;
	
	}

	/*建立临时文件保存目录列表*/
	File *tmpfile;
	char tmpbuff[2048];
	int filesize=0;
	char *uri=wctl->conn.con_req.uri;
	tmpfile=tmpfile();

	/*标题部分*/
   sprintf(tmpbuff,
			"%s%s%s",
			"<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 3.2Final //EN\">\n<HTML><HEAD><TITLE>",
			uri,
			"</TITLE></HEAD>\n");
	fprintf(tmpfile,"%s",tmpbuff);
	filesize +=strlen(tmpbuff);

	//part of identification
	sprintf(tmpbuff,
		"%s%s%s",
		"<BODY><H1>Index of:"
		"uri",
		"</H1> <HR><P><I>Date:</I> <I>Size: </I></P><HR>");
	fprintf(tmpfile,"%s",tmpbuff);
	filesize +=strlen(tmpbuff);
	
	/* 读取目录中的文件列表 */

	struct dirent *de;
#define PATHLENGTH 2048
	char path[PATHLENGTH];
	char tmpath[PATHLENGTH];
	char linkname[PATHLENGTH];
	struct stat fs;
	
  	strcpy(path,rpath);
	if(rpath[strlen(rpath)]!='/'){
		rpath[strlen(rpath)]='/';
	}
	
	while((de==readdir(dir))!=NULL)
	{
		menset(tmpath,0,sizeof(tmpath));
		menset(linkname,0,sizeof(linkname));
		if(strcmp(de->d_name,"."))/*不是当前目录*/
		{
			if(strcmp(de->dname,".."))
			{
				strcpy(linkname,de->d_name);
			}
			else
			{
				strcpy(linkname,"Parent Directory");
			}
			
			sprintf(tmpath,"%%",path,de->d_name);
			stat(tmpath,&fs);
			if(S_ISDIR(fs.st_mode)){
				sprintf(tmpbuff,"<A HREF=\"%s/\">%s/</A><BR>\n",de->d_name,tmpath);
			}
			else{
				char size_str[32];
				off_t size_int;

				size_int=fs.st_size;
				if(size_int<1024)
					sprintf(size_str,"%d bytes",(int) size_int);
				else if (size_int<1024*1024)
					sprintf(size_str,"%1.2f Kbytes",(float) size_int/1024);
				else
					sprintf(size_str,"%1.2 Mbytes",(float) size_int/(1024*1024));

			}
			
			/*将形成的字符串写入临时文件*/
			fprintf(tmpfile,"%s",tmpbuff);
			filesize +=strlen(tmpbuff);
		}
	}

	fs.st_ctime=time(NULL);
	fs.st_mtime=time(NULL);
	fs.st_size=filesize;
	fseek(tmpfile,(long) 0,SEEK_SET);/*移动文件指针到头部*/

	{

		DBGPRINT("==>Method_DoGet\n");
		struct conn_response *res=&wctl->conn.con_res;
		struct conn_resquest *req=&wctl->conn.con_req;
		char path[URI_MAX];
		memset(path,0,URI_MAX);
		
		size_t n;
		unsigned long r1,r2;
		char *fmt="%a,%d %b %Y %H:%M:%S GMT";

		/*需要确定的参数*/
		size_t status=200;
		char *msg="ok";
		char date[64]="";
		char lm[64]="";
		char etag[64]="";
		big_int_t cl;
		char range[64]="";
		struct mine_type *mine=NULL;

		/*当前时间*/
		time_t t=time(NULL);
		(void) strftime(date,
						sizeof(date),
						fmt,
						localtime(&t));
		(void) snprintf(etag,
						sizeof(etag),
						"%lx.lx%",
						(unsigned long)res->fsate.st_mtime,
						(unsigned long)res->fsate,st_size);
		/*发送的MIME类型*/
		mine=Mine_Type(req->uri,strlen(req->uri),wctl);
		/*内容长度*/
		cl=(big_int_t)res->fsate.st_size;

		memset(range,0,sizeof(range));
		n=-1;
		if(req->ch.range.v_vec.len>0){
			printf("request range :%d\n",req->ch.range.v_vec.len);
			n=sscanf(req->ch.range.v_vec.ptr,"bytes=%lu-%lu",&r1,&r2);
				
		}
		
		printf("n:%d\n",n);
		if(n>0){

			status=206;
			(void)sseek(res->fd,r1,SEEK_SET);
			cl=n==2?r2-r1+1:cl-r1;
			(void)snprintf(range,
							sizeof(range),
							"Content-Range:bytes %lu-%lu/%lu\r\n",
							r1,
							r1+c1-1,
							(unsigned long) res->fsate.st_size);
			
			msg="Partial Content";
		}

		memset(res->res.ptr,0,sizeof(wctl->conn.dres));
		snprintf(res->res.ptr,
				sizeof(wctl->conn.dres),
				"HTTP/1.1 %d %s \r \n"
				"Date:%s\r\n"
				"Last_Modified:%s\r\n"
				"Etag:\"%s\"\r\n"
				"Content-Types:%.*s\r\n"
				"Content-Length:%lu\r\n"
				"Accept-Ranges:bytes\r\n"
				"%s\r\n",
				status,
				msg,
				date,
				lm,
				etag,
				strlen(mine->mime_type),
				mine->mime_type,
				cl,
				range);
		res->cl=cl;
		res->status=status;
	}











}



















