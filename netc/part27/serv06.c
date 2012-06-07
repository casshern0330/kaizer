#include "unpthread.h"

/*
 *@description:�������߳�,�����̺߳��������̴߳���connfd,ÿ���ͻ������ʱ�򴴽�һ���߳�
 */
int main(int argc,char *argv)
{
	int	listenfd,connfd;
	socklen_t	clilen,addrlen;
	pthread_t	ptid;
	struct	sockaddr *cliaddr;
	void *doit(void *);
	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage: serv06 [<host>] <#port>");
	cliaddr = malloc(addrlen);
	Signal(SIGINT,sig_int);
	for(;;)
	{
		clilen = addrlen;
		connfd = accept(listenfd,cliaddr,&clilen);
		pthread_create(&ptid,NULL,&doit,(void*)&connfd);
	}

}

void* doit(void *arg)
{
	int connfd;
	connfd = (int)*arg;
	pthread_detach(pthread_self());
	web_child(connfd);
	Close(connfd);
	return (NULL);
}
