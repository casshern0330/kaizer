#include "unp.h"

static struct flock lock_it,unlock_it;
static int fd = -1;

/*
 * @descriptor:主进程创建listenfd,子进程创建connfd,次用文件锁方式保证子进程中的accept并发安全
 */
void my_lock_init(char *filename)
{
	char	lock_file[1024];
	
	strncpy(lock_file,filename,sizeof(lock_file));
	//创建一个零时文件
	Mktemp(lock_file);

	fd = open(lock_file,O_CREAT | O_WRONLY,FILE_MODE);
	
	unlink(lock_file);

	bzero(lock_it,sizeof(struct flock));
	bzero(unlock_it,sizeof(struct flock));

	lock_it.l_type = F_WRLCK;
	lock_it.l_wheel = SEEK_SET;
	lock_it.l_start = 0;
	lock_it.l_len = 0;

	unlock_it.l_type = F_UNLCK;
	unlock_it.l_wheel = SEEK_SET;
	unlock_it.l_start = 0;
	unlock_it.l_len = 0;
}


void my_lock_wait()
{
	int	rc;
	while((rc = fcntl(fd,F_SETLKW,&lock_it)) < 0)
	{
		if(errno == EINTR)
			continue;
		else
			err_quit("fcntl error for my_lock_wait");
	}
}

void my_lock_release()
{
	int rc;
	if((rc = fcntl(fd,F_SETLKW,&unlock_it)) < 0)
		err_sys("fcntl error for my_lock_release");
}

void child_main(int fd,socklen_t addrlen)
{
	socklen_t clilen;
	struct sockaddr *cliaddr;
	int	connfd;

	cliaddr = malloc(addrlen);
	for(;;)
	{
		clilen = addrlen;
		my_lock_wait();
		if((connfd = accept(listenfd,cliaddr,&clilen)) < 0)
		{
			if(errno = EINTR)
				continue;
			else
				err_quit("accept error!");
		}
		my_lock_release();
		web_child(connfd);
	}
}


#include "mmap.h"

/*
 *@description:返回一段共享内存空间
 */
long *meter(int nchildren)
{
	int	fd;
	long	*ptr;
#ifdef	MAP_ANON
	ptr = Mmap(0,nchildren*sizeof(long),PROT_READ | PROT_WRITE,
			MAP_ANON | MAP_SHARED,-1,0);
#else
	fd = Open("/dev/zero",O_RDWR,0);
	ptr = Mmap(0,nchildren*sizeof(long),PROT_READ | PROT_WRITE,
			MAP_SHARED,fd,0);
#endif
	return (ptr);
}
