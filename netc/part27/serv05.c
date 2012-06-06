#include "unp.h"

typedef struct{
	pid_t	child_pid;
	int	child_pipefd;
	int	child_status;
	long	child_count;
}Child;

static Child *cptr;

int main(int argc,char *argv[])
{
	int	listenfd,connfd,maxfd;
	int	i,nchildren;
	struct sockaddr *cliaddr;
	socklen_t	clilen,addrlen;
	fd_set	rset,masterset;

	if(argc == 3)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 4)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage: serv05 [<host>] <#port> <#nchildren>");

	nchildren = atoi(argv[argc-1]);

	cptr = calloc(sizeof(Child),nchildren);

	maxfd = -1;
	FD_ZERO(&
	for(i=0;i<nchildren;i++) {
		child_make(cptr[i]);
		FD_SET(cptr[i].child_pipefd,&masterset);
		maxfd = max(maxfd,cptr[i]->child_pipefd);
	}
}


void child_make(Child *cptr){
	int	sockfd[2];
	pid_t	pid;

	socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd);

	if((pid = fork()) > 0){
		
		cptr->child_pid = pid;
		cptr->child_pipefd = sockfd[0];
		cptr->child_status = 0;
		cptr->child_count = 0;
	}
}
