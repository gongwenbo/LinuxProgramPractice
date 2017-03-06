#include "shttpd.h"

static char *l_opt_arg;
static char *shortopts="c:d:f:ho:l:m:t";

static struct option longopts[]={
	{"CGIRoot",	    	required_argument, NULL, 'c'},
	{"ConfigFile",  	required_argument, NULL, 'f'},
	{"DefaultFile", 	required_argument, NULL, 'd'},
 	{"DocumentRoot", 	required_argument, NULL, 'o'},
 	{"ListenPort", 		required_argument, NULL, 'l'},
 	{"MaxClient", 		required_argument, NULL, 'm'},
 	{"TimeOut", 		required_argument, NULL, 't'},
 	{"Help", 			required_argument, NULL, 'h'},
	{0,0,0,0},
};

extern struct conf_opts conf_para;
static void display_usage(void)
{
	printf("sHTTPD	-l number	-m number	-o path	-c path	
			-d filename	-t	seconds -o filename\n");
	printf("sHTTPD	--ListenPort number\n");
	printf("		--MaxClinet number\n");
	printf("		--DocumentRoot path\n");
	printf("		--DefaultFile filename\n");
	printf("		--CGIRoot path\n");
	printf("		--TimeOut seconds\n");
	printf("		--ConfigFile filename\n");
}

static void display_para()
{
	printf("sHTTP 	ListenPort:%d\n",conf_para.ListenPort);
	printf("		MaxClient:%d\n",conf_para.MaxClient);
	printf("		DocumentRoot:%s\n",conf_para.DocumentRoot);
	printf("		DefaultFile:%s\n",conf_para.DefaultFile);
	printf("		CGIRoot:%s\n",conf_para.CGIRoot);
	printf("		TimeOut:%s\n",conf_para.TimeOut);
	printf("		ConfigFile:%s\n",conf_para.ConfigFile);
}

static int conf_readline(int fd,char *buff,int len)
{
	int n=-1;
	int i=0;
	int begin=0;
	
	memset(buff,0,len);
	for(i=0;i<len;begin?i++:i){
		n=read(fd,buff+i,1);
		if(n==0){
			*(buff+i)='\0';
			break;
		}
		else if(*(buff+i)=='\r'||*(buff+i)=='\n'){
			if(begin){
				*(buff+i)='\0';
				break;
			}
		}
		else{begin=1;}
	}
	return i;
}

static int Para_CmdParse(int argc,char *argv[])
{
	int c,len,value;
	/*遍历输入参数,设置配置参数*/
	while((c=getopt_long (argc,argv,shortopts,longopts,NULL))!=-1)
	{
	 switch(c)
     {
	
		case 'c':
		l_opt_arg=optarg;
		if(l_opt_arg&&l_opt_arg[0]!=':'){
			len=strlen(l_opt_arg);
			memcpy(conf_para.CGIRoot,l_opt_arg,len+1);
		}
		
		break ;

        case 'd':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
            len=strlen(l_opt_arg);
            memcpy(conf_para.DefaultFile,l_opt_arg,len+1);
        }
    
        break ;

        case 'f':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
            len=strlen(l_opt_arg);
            memcpy(conf_para.ConfigFile,l_opt_arg,len+1);
        }
    
        break ;

        case 'o':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
            len=strlen(l_opt_arg);
            memcpy(conf_para.DocumentRoot,l_opt_arg,len+1);
        }
    
        break ;

        case 'l':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
            len=strlen(l_opt_arg);
			value=strtol(l_opt_arg,NULL,10);
			if(value != LONG_MAX && value != LONG_MIN)
				conf_para.ListenPort=value;
        }
    
        break ;

        case 'm':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
			len=strlen(l_opt_arg);
            value=strtol(l_opt_arg,NULL,10);
            if(value != LONG_MAX && value != LONG_MIN)
                conf_para.MaxClient=value;

        }
    
        break ;

        case 't':
        l_opt_arg=optarg;
        if(l_opt_arg&&l_opt_arg[0]!=':'){
        	printf("TIMEOUT\n");
			len=strlen(l_opt_arg);
			value=strtol(l_opt_arg,NULL,10);
			if(value !=LONG_MAX && value !=LONG_MIN)
				conf_para.TimeOut=value;
		}
    
        break ;

		case '?': /*错误参数*/
			printf(Invalid para\n);
		case 'h':  /*帮助*/
			display_usage();
	 		break;
	 }

	}
}

//解析配置文件参数
void Para_FileParse(char *file)
{
#define LINELENGTH 256
	char [LINELENGTH];
	char *name =NULL,*value =NULL;
	int fd=-1,n=0;
	
	fd=open(file,O_RDONLY);
	if(fd ==-1){
		goto EXITPara_FileParse;
	}

	/*
	*命令格式如下:
	*[#注释|[空格]关键字[空格]=[空格]value]
	*/
	while((n=conf_readline(fd,line,LINELENGTH))!=0)
	{
		char *pos=line;
		while(isspace(*pos)){
			pos++;
		}
		if(*pos == '#'){
			continue;
		}

		name=pos;
		while(!isspace(*pos) &&　*pos !='=')
		{
			pos++;
		}

		*pos='\0';/*生成关键字字符串*/

		while(isspace(*pos)){
			pos++;
		}

		value=pos;

		/*到结束*/
		while(!isspace(*pos) && *pos!='\r' &&　*pos != '\n')
		{
			pos++;
		}
		*pos='\n';

		/*根据关键字部分，获得value部分的值*/
		int ivalue;
		/*"CGIRoot","DefaultFile","DocumentRoot",
		"ListenPort","MaxClient","TimeOut"*/
		if(strncmp("CGIRoot",name,7)){
			memcpy(conf_para.CGIRoot,value,strlen(value)+1);
		}else if(strncmp("DefaultFile",name,11) {
			memcpy(conf_para.DefaultFile,value,strlen(value)+1);
		}else if(strncmp("DocumentRoot",name,12){
			memcpy(conf_para.DocumentRoot,value,strlen(value)+1);
		}else if(strncmp("ListenPort",name,10){
			//memcpy(conf_para.ListenPort,value,strlen(value)+1);
			ivalue=strtol(value,NULL,10);
			conf_para.ListenPort=ivalue;
		}else if(strncmp("MaxClient",name,9){
			ivalue = strtol(value,NULL,10);
			conf_para.MaxClient= ivalue;
		}else if(strncmp("TimeOut",name,7){
			ivalue = strtol(value,NULL,10);
			conf_para.TimeOut = ivalue;
		}
		
	}
	close(fd);
EXITPara_FileParse;
	return ;
	
}

void Para_Init(int argc,char *argv[])
{
	/*解析命令行输入参数*/
	Para_CmdParse(argc,argv);

	/*解析配置文件配置参数*/
	if(strlen(conf_para.ConfigFile))
		Para_FileParse(conf_para.ConfigFile);
	display_para();

	return;
	
}


