#include "unpthread.h"

typedef{
	pthread_t thread_tid;
	long	thread_count;
}Thread;

Thread *tptr;

int	listenfd,nthreads;
socklen_t	addrlen;
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;

/*
 *@description:主进程获取listenfd,子线程进行accept
 */
int main(int argc,char *argv[])
{
	int nchildren,i;
	socklen_t	addrlen;
	
	if(argc == 3)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 4)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage:serv07 [<host>] <#port> <#nchildren>");
	nchildren = atoi(argv[argc-1]);

	tptr = calloc(nchildren,sizeof(Thread));

	for(i=0;i<nchildren;i++)
		thread_make(i);
	while(1)
		pause();
}


void thread_make(int i)
{
	void *(*thread_main)(void *);
	thread_create(&(tptr[i].thread_tid),NULL,&thread_main,(void*)&i);	
}

void* thread_main(void *arg)
{
	int i,connfd;
	socklen_t clilen;
	struct sockaddr *cliaddr;
	i = *((int *)arg);
	cliaddr = malloc(addrlen);
	pthread_detach(pthread_self());
	
	for(;;)
	{
		clilen = addrlen;
		pthread_mutex_lock(&mlock);
		connfd = accept(listenfd,cliaddr,&clilen);
		pthread_mutex_unlock(&mlock);
		tptr[i].thread_count++;
		child_web(connfd);
		Close(connfd);
	}
}
