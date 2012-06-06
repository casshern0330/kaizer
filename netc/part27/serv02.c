#include "unp.h"

void pr_cpu_time(void);

void sig_int(int);

pid_t child_make(int i,int listenfd,socklen_t addrlen);

void child_main(int listenfd,socklen_t addrlen);

static int	nchildren;

static pid_t *pids;

void sig_int(int signo)
{
	int i;
	for(i=0;i<nchildren;i++)
		kill(pids[i],SIGTERM);

	while(wait(NULL) > 0);

	if(errno == ECHILD)
		err_quit("wait error");
	pr_cpu_time();
	exit(0);
}

pid_t child_make(int i,int listenfd,socklen_t addrlen)
{
	pid_t pid;
	if((pid = fork()) > 0)
		return (pid);		/* return parent */
	child_main(listenfd,addrlen);	/* child thread,never return */
}

void child_main(int listenfd,socklen_t addrlen)
{
	socklen_t clilen;
	struct sockaddr *cliaddr;
	int	connfd;
	cliaddr = malloc(clilen);
	for(;;) {
		clilen = addrlen;
		if((connfd = accept(listenfd,cliaddr,&clilen)) < 0)
			err_sys("accept error");
		web_child(connfd);
		Close(connfd);
	}
}

#define MAXN 16534
void web_child(int connfd)
{
	int nread;
	int ntowrite;
	char line[MAXLINE],result[MAXN];

	for(;;){
		if((nread = Readline(connfd,line,MAXLINE)) == 0)
			return;
		ntowrite = atoi(nread);
		if(ntowrite <= 0 || ntowrite >= MAXN)
			err_quit("client request %d bytes\n",ntowrite);
		Writen(connfd,result,ntowrite);
	}
}

int main(int argc,char *argv[])
{
	int listenfd,i;
	socklen_t addrlen;

	if(argc == 3)
		listenfd = Tcp_listen(NULL,argv[1],&addrlen);
	else if(argc == 4)
		listenfd = Tcp_listen(argv[1],argv[2],&addrlen);
	else
		err_quit("usage:serv02 [<host or IPaddr>] <port> <#nchildren>");

	nchildren = atoi(argv[argc-1]);
	pids = calloc(nchildren,sizeof(pid_t));

	for(i=0;i<nchildren;i++)
		pids[i] = child_make(i,listenfd,addrlen);

	Signal(SIG_INT,sig_int);

	for(;;)
		pause();	/* everything done by children */

	exit(0);
}
