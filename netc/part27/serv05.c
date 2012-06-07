#include "unp.h"

typedef struct{
	pid_t	child_pid;
	int	child_pipefd;
	int	child_status;
	long	child_count;
}Child;

static Child *cptr;

pid_t child_make(int i,int listenfd,socklen_t addrlen);
{
	int	sockfd[2];
	pid_t pid;
	socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd);

	if((pid = fork()) > 0){
		Close(sockfd[0]);
		cptr[i].child_pid = pid;
		cptr[i].child_pipefd = sockfd[1];
		cptr[i].child_status = 0;
		cptr[i].child_count = 0;
		return (pid);
	}

	Close(sockfd[1]);
	child_main(i,sockfd[0],addrlen);
}

void child_main(int i,int pipefd,socklen_t addrlen)
{
	int connfd;
	size_t n;
	char c;

	for(;;){
		if((n = Read_fd(pipefd,&c,sizeof(c),&connfd)) == 0)
			err_quit("get discriptor error.\n");
		if(connfd < 0)
			err_quit("no descriptor from read_fd");
		web_child(connfd);
		Close(connfd);
		Write(pipefd,"",1);
	}
}

#define MAXN 16354
void web_child(int connfd)
{
	char	line[MAXLINE],result[MAXN];
	int	nbytes;
	size_t	n;
	
	for(;;)
	{
		if((n = Readline(connfd,line,MAXLINE)) == 0)
			err_quit("read error");
		nbytes = atoi(line);
		if(nbytes <=0 || nbytes >= MAXN)
			err_quit("client request %d bytes",nbytes);
		Write(connfd,result,nbytes);
	}	
}

/*
 * @descriptor:主进程创建listenfd,住进程负责创建connfd,采用发送带外数据的方式，传递connfd给子进程
 */
void main(int argc,char *argv[])
{
	int	listenfd,connfd,i,maxfd,nchildren,nsel,navail;
	char	rc;
	fd_set	rset,masterset;
	socklen_t	addrlen,clilen;
	struct sockaddr *cliaddr;

	if(argc == 3)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 4)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage:serv06 [<host>] <port> <#nchildren>");

	maxfd = listenfd;
	cliaddr = malloc(addrlen);
	nchildren = atoi(argv[argc-1]);
	navail = nchildren;
	cptr = calloc(nchildren,sizeof(Child));

	FD_ZERO(&masterset);
	for(i = 0 ; i<nchildren;i++)
	{
		child_make(i,listenfd,addrlen);
		FD_SET(cptr[i].child_pipefd,&masterset);
		maxfd = max(maxfd,cptr[i].child_pipefd);
	}

	Signal(SIGINT,sig_int);

	for(;;)
	{
		rset = masterset;
		if(navail <= 0)
			FD_CLR(listenfd,&rset);
		nsel = Select(maxfd+1,rset,NULL,NULL,NULL);

		if(FD_ISSET(listenfd,&rset)) 
		{
			clilen = addrlen;
			connfd = accept(listenfd,cliaddr,&clilen);
			
			for(i=0;i<nchildren;i++)
				if(cptr[i].child_status == 0)
					break;

			if(i == nchildren)
				err_quit("no available children process");

			cptr[i].child_status = 1;
			cptr[i].child_count++;
			if(Write_fd(cptr[i].child_pipefd,"",1,connfd) == 0)
				err_quit("write connfd to %d child error\n",i);

			Close(connfd);
			navail--;
			if(--nsel <= 0)
				continue;
		}

		for(i=0;i<nchildren;i++)
		{
			if(FD_ISSET(cptr[i].child_pipefd,&rset))
			{
				if(Read(cptr[i].child_pipefd,&rc,1) == 0)
					err_quit("read error from child %d",i);
				cptr[i].child_status = 0;
				navail++;
				if(--nsel <= 0)
					break;			
			}
		}
	}
}


