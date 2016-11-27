广播的代码无法执行，只是一个框架而已。
客户端架构：
1 sock = socket(AF_INET, SOCK_DGRAM, 0);/*建立数据报套接字*/
2 ioctl(sock,SIOCGIFBRDADDR,&ifr) /*发送命令，获取网络接口的广播地址*/
3 ret = setsockopt(sock,SOL_SOCKET,SO_BROADCAST,&so_broadcast,sizeof so_broadcast);  /*设置套接字文件描述符sock为可以进行广播操作*/
4 ret = sendto(sock,IP_FOUND,strlen(IP_FOUND),0,(struct sockaddr*)&broadcast_addr,sizeof(broadcast_addr));   /*广播发送服务器地址请求*/
5 count = recvfrom( sock, buff, BUFFER_LEN, 0,( struct sockaddr*) &from_addr, &from_len );   /*接收数据*/

服务器架构：
1 sock = socket(AF_INET, SOCK_DGRAM, 0);	/*建立数据报套接字*/
2 local_addr.sin_port = htons(MCAST_PORT);		/*侦听端口*/
3 ret = bind(sock, (struct sockaddr*)&local_addr, sizeof(local_addr)); 
4 recvfrom( sock, buff, BUFFER_LEN, 0, ( struct sockaddr*) &from_addr, &from_len );
5 if( strstr( buff, IP_FOUND ) ) /*判断是否吻合*/
6 /*发送给客户端*/
  count = sendto( sock, buff, strlen( buff ),0,( struct sockaddr*) &from_addr, from_len );
