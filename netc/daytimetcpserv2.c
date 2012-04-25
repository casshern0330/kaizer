#include "unp.h"

int main(int argc,char *argv[])
{
	int listenfd,connfd;
	socklen_t addrlen,len;
	struct sockaddr *cliaddr;
	char buffer[MAXLINE];
	time_t ticks;

	if(argc == 2)
		listenfd = tcp_listen(NULL,argv[1],&addrlen);
	if(argc == 3)
		listenfd = tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage:daytimetcpsrv2:[<host>] <service or port>]");
	
	cliaddr = malloc(addrlen);

	for (;;) {
		len = addrlen;
		connfd = accpet(listenfd,cliaddr,&len);
		printf("connection from %s\n",Sock_ntop(cliaddr,len));

		ticks = time(NULL);
		snprintf(buffer,sizeof(buffer),"%.24s\r\n",ctime(&ticks));
		Writen(connfd,buffer,strlen(buffer));
		Close(connfd);
	}
}
