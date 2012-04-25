#include "unp.h"

void sig_chld(int signo);

typedef void (*SigFunc)(int);

int main(int argc,char *argv[])
{
	int listenfd,sockfd;
	struct sockaddr cliaddr;
	socklen_t servlen,clilen;
	pid_t pid;
	SigFunc old;
	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],&servlen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],&servlen);
	else
		err_quit("usage: server01 [<hostname>] <service>\n");
	old = signal(SIGCHLD,sig_chld);
	while(1){
		clilen = servlen;
		if((sockfd = accept(listenfd,&cliaddr,&clilen)) < 0){
			if(errno == EINTR)
				continue;
			else
				err_sys("error get tcp connection\n");
		}
		if((pid = fork()) == 0) {
			Close(listenfd);
			str_echo(sockfd);
			Close(sockfd);
			exit(0);
		}
		Close(sockfd);
	}
	signal(SIGCHLD,old);
	exit(0);
}

void str_echo(int sockfd)
{
	int fd,n;
	char recvmsg[MAXLINE];
	fd = sockfd;
	while(1){
		if((n = Readline(fd,recvmsg,MAXLINE)) == 0)
			return;
		recvmsg[n] = 0;
		Fputs(recvmsg,stdout);
		Writen(fd,recvmsg,strlen(recvmsg));
	}
}

void sig_chld(int signo)
{
	int status;
	pid_t pid;
	char str[100];
	while((pid = waitpid(-1,&status,WNOHANG) > 0)){
		snprintf(str,sizeof(str),"child %d has terminated\n",pid);
		perror(str);
	}
	return;
}
