#include "unp.h"
#include <sys/resource.h>

#ifdef HAVE_GETRUSAGE_PROTO
int	getrusage(int,struct rusage *);
#endif

void pr_cpu_time(void)
{
	struct rusage myusage,childusage;
	double sys,user;

	if(getrusage(RUSAGE_SELF,&rusage) < 0)
		err_quit("getrusage error");
	if(getrusage(RUSAGE_CHILDREN,&childusage) < 0)
		err_quit("getrusage error");

	user = (double)myusage.ru_utime.tv_sec +
			myusage.ru_utime.tv_usec/1000000.0;
	user += (double)childusage.ru_utime.tv_sec +
			childusage.ru_utime.tv_usec/1000000.0;
	sys = (double)myusage.ru_stime.tv_sec +
			myusage.ru_stime.tv_usec/1000000.0;
	sys += (double)childusage.ru_stime.tv_sec +
			childusage.ru_stime.tv_usec/1000000.0;

	printf("\nuser time = %g,sys time = %g\n",user,sys);
}


int sig_int(int signo)
{
	pr_cpu_time();
	exit(0); 
}

int sig_chld(int signo)
{
	pid_t pid;
	int	stat;
	while((pid = waitpid(-1,&stat,WNOHANG) > 0)
			printf("child %d terminated\n",pid);
	exit(0);
}

#define MAXN 16384
void child_web(int connfd)
{
	int ntowrite;
	char	line[MAXLINE],result[MAXN];
	ssize_t	nread;
	for(;;) {
		if((nread = Readline(connfd,line,MAXLINE)) == 0)
			return;
		ntowrite = atoi(line);
		if(ntowrite <= 0 || ntowrite >= MAXN)
			err_quit("client request for %d bytes\n",ntowrite);
		Writen(connfd,result,ntowrite);
	}
}


int main(int argc,char *argv[]) {
	int listenfd,connfd;
	pid_t pid;
	socklen_t servlen,clilen;
	struct sockaddr *cliaddr;

	if(argc == 2)
		listenfd =Tcp_listen(NULL,argv[1],&servlen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[0],argv[1],&servlen);
	else
		err_quit("usage: serv01 [<host or IPAddr>] <#port>");
	
	Signal(SIGCHLD,sig_chld);
	Signal(SIGINT,sig_int);
	cliaddr = malloc(servlen);
	for(;;) {
		clilen = servlen;
		if((connfd = accept(listenfd,cliaddr,&clilen)) < 0){
			if(errno == EINTR)
				continue;
			else
				err_quit("accept error");
		}
		
		if((pid = fork()) == 0) {
			Close(listenfd);
			child_web(connfd);
			exit(0);
		}

		Close(connfd);
	}
}


