#include "unp.h"
#include <sys/mmap.h>

static pthread_mutex_t *mptr;

/*
 *@descriptor:主进程创建listenfd,子进程采用互斥锁方式保证accept的并发安全
 */
void my_lock_init(char *pathname)
{
	int fd;
	pthread_mutexattr_t mattr;

	fd = Open("/dev/zero",O_RDWR,0);
	mptr = Mmap(0,sizeof(pthread_mutex_t),PROT_READ | PROT_WRITE,
			MAP_SHARED,fd,0);

	//关闭描述子
	Close(fd);

	pthread_mutexattr_init(&mattr);
	pthread_mutexattr_setpshared(&mattr,PTHREAD_PROCESS_SHARED);
	pthread_mutex_init(mptr,&mattr);
}

void my_lock_wait()
{
	pthread_mutex_lock(mptr);
}

void my_lock_release()
{
	pthread_mutex_unlock(mptr);
}


void child_main(int listenfd,socklen_t addrlen)
{
	int connfd;
	struct sockaddr *cliaddr;
	socklen_t clilen;

	cliaddr = malloc(addrlen);
	for(;;)
	{
		clilen = addrlen;
		my_lock_wait();
		connfd = accept(listenfd,cliaddr,&clilen);
		my_lock_release();
		web_child(connfd);	
	}
}
