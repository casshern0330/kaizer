#include "unpthread.h"

typedef struct{
	pthread_t	thread_pid;
	long	thread_count;
}Thread;

Thread *tptr;

#define MAXNCLI 32

int clifd[MAXNCLI],iput,iget;

pthread_mutex_t	clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t	clifd_cond = PTHREAD_COND_INITIALIZER;

static int	nthread;

int main(int argc,char *argv[])
{
	int	listenfd,connfd,nchildren,i;
	socklen_t	addrlen,clilen;
	struct sockaddr *cliaddr;

	void sig_int(int),thread_make(int);

	if(argc == 3)
		listenfd = Tcp_listen=(NULL,argv[1],&addrlen);
	else if(argc == 4)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage: serv08 [<host>] <#port> <#nchildren>");

	nchildren = atoi(argv[argc-1]);
	tptr = calloc(nchildren,sizeof(Thread));
	iput = iget = 0;
	for(i=0;i<nchildren;i++)
		child_make(i);

	cliaddr = malloc(addrlen);
	for(;;)
	{
		clilen = addrlen;
		connfd = accept(listenfd,cliaddr,&clilen);
		pthread_mutex_lock(&clifd_mutex);
		clifd[iput] = connfd;
		if(++iput = MAXNCLI)
			iput = 0;
		if(iput == iget)
			err_quit("iput == iget == %d\n",iput);
		pthread_cond_signal(&clifd_cond);
		pthread_mutex_unlock(&clifd_mutex);
	}
}

void child_make(int i)
{
	void *(*child_main(void *));
	pthread_create(&(cptr[i].thread_pid),NULL,child_main,(void*)&i);
	return;
}

void* child_main(void *arg)
{
	void web_child(int);
	int connfd;
	int i = (int)*arg;
	for(;;)
	{
		pthread_mutex_lock(&clifd_mutex);
		while(iget == iput)
			pthread_cond_wait(&clifd_cond,&clifd_mutex);
		connfd = clifd[iget];
		if(++iget == MAXNCLI)
			iget = 0;
		pthread_mutex_unlock(&clifd_mutex);	
		tptr[i].thread_count++;
		web_child(connfd);
		Close(connfd);
	}
}
