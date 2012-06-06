#include "unpthread.h"

int main(int argc,char *argv[])
{
	int listenfd,connfd;
	void sig_int(int);
	void *doit(void *);
	pthread_t tid;
	socklen_t clilen,addrlen;
	struct sockaddr *cliaddr;

	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else 
		err_quit("usage: serv06 [<host>] <#port>");

	cliaddr = malloc(addrlen);
	signal(SIGINT,sig_int);
	for(;;) {
		clilen = addrlen;
		connfd = accept(listenfd,cliaddr,&clilen);
		pthread_create(&tid,NULL,doit,(void*)connfd);
	}

	Close(listenfd);
}

void *doit(void connfd)
{
	void web_child();
	pthread_detach(pthread_self());

	web_child((int)connfd);
	Close((int)connfd);
	return (NULL);
}
