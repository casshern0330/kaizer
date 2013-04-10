#include "unpthread.h"

static void doit(void *);

void main(int argc,char *argv[])
{
	int listenfd,acceptfd;
	socklen_t addrlen,len;
	int	*lptr;
	struct sockaddr *cliaddr;

	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage:tcpserver01 [<host>] <service or port>");
	cliaddr = malloc(addrlen);
	for(;;){
		lptr = Malloc(sizeof(int));
		len = addrlen;
		*lptr = Accept(listenfd,cliaddr,&len);
		Pthread_create(NULL,NULL,&doit,lptr);
	}
}


static void doit(void *arg)
{
	int	connfd;
	connfd	= *((int *)arg);

	free(arg);
	Pthread_detach(Phtread_self());
	str_echo(connfd);
	Close(connfd);
	return (NULL);
}
