#include "shttpd.h"
/*
[
http/1.1 200 OK
date:thu,11 dec 2008 11:25:33 GMT
last-modidied :wed, 12 NOv 2008 09:00:01 GMT
etag:"491a2a91.2afe"
Content-Type:text/plain
Content-Length :11006
Accept-Ranges:bytes
]
*/
static int Method_DoGet(struct worker_ctl *wctl)
{
	DBGPRINT("==>Method_DoGet\n");
	struct conn_response *res=&wctl->conn.con_res;
	struct conn_request *req=&wctl->conn.con_req;
	char path[URI_MAX];

	size_t n;
	unsigned long r1,r2;
	char *fmt="%a,%d %b %Y %H:%M:%S GMT";

	/*需要确定的参数*/
	size_t status-200;
	char *msg="OK";
	char date[64]="";
	char lm[64]="";
	char etag[64]="";
	big_int_t c1;
	char range[64]="";
	struct mine_type *mine=NULL;

	/*当前时间*/
	time_t t=time(NULL);
	(void) strftime(date,
				sizeof(date),
				fmt,
				localtime(&t));

	(void) strftime(lm,
				sizeof(lm),
				fmt,
				localtime(&res->fsate.st_mtime));
	(void) snprintf(etag,
				sizeof(etag),
				"%lx.%lx",
				(unsigned long)res->fsate.st_mtime,
				(unsigned long)res->fsate,st_size);
	/*发送的MIME类型*/
	mine=Mine_Type(req->uri,strlen(req-uri),wctl);
	c1=(big_int_t) res->fsate.st_size;

    /*范围range*/
	memset(range,0,sizeof(range));
	n=-1;
	if(req->ch.range.v_vec.len>0)
	{
		printf("requset range :%d\n",req->ch.range.v_vec.len);
		n=sscanf(req->ch.range.v_vec.ptr,"bytes=%lu-%lu",&r1,&r2);
	}
	
	printf("n:%d\n",n);
	if(n>0)
	{
		status=206;
		lseek(res->fs,r1,SEEK_SET);
		c1=n==2?r2-r1+1:c1-r1;
		(void) snprintf(range,
					sizeof(range),
					"content-range:bytes %lu-%lu/%lu\r\n",
					r1,
					r1+c1-1,
					(unsigned long)res->fsate.st_size);
		msg="partial content"
	}

	memset(res->res.ptr,0,sizeof(wctl->conn.dres));
	snprintf(
		res->res.ptr,
		sizeof(wctl->conn.dres),
		"HTTP/1.1 %d %s \r\n"
		"date:%s\r\n"
		"Last-Modified:%s\r\n"
		"Etag:\"%s\"\r\n"
		"Content-Type:%.*s\r\n"
		"Content-Length:%lu\r\n"
		"Accept-Ranges:bytes\r\n"
		"%s\r\n",
		status,
		msg,
		date,
		lm,
		etag,
		strlen(mine->mime_types),
		c1,
		range);
	res->c1=c1;
	res->status=status;
	printf("content length:%d,status:%d\n",res->c1,res->status);
	DBGPRINT("<==Method_DoGet\n");
	return 0;
	
}

static int Method_DoPost(struct worker_ct1 *wct1)
{
	return 0;
}

static int Method_DoHead(struct worker_ct1 *wct1)
{
	Method_DoGet(wct1);
	close(wct1->conn.con_res.fd);
	wct1->conn.con_res.fd=-1;

	return 0;
}

static int Method_DoPut(struct worker_ct1 *wct1)
{
	return 0;
}

static int Method_DoDelete(struct worker_ct1 *wct1)
{
    return 0;
}

static int Method_DoCGI(struct worker_ct1 *wct1)
{
    return 0;
}

static int Method_DoList(struct worker_ct1 *wct1)
{
    return 0;
}

static int Method_Do(struct worker_ct1 *wct1)
{
	DBGPRINT("==>Method_Do\n");
	if(0)
		Method_DoCGI(wct1);
	switch(wct1->conn.con_req.method)
	{
		case METHOD_PUT:
			Method_DoPut(wct1);
			break;
	
		case METHOD_DELETE:
			Method_DoDelete(wct1);
			break;

		case METHOD_GET:		
			Method_DoGet(wct1);
			break;
		
		case METHOD_POST:
			Method_DoPost(wct1);
			break;

		case METHOD_HEAD:
			Method_DoHead(wct1);
			break;

		default:
			Method_DoList(wct1);
			
	}

	DBGPRINT("<==Method_Do\n");
}


