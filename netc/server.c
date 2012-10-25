#include "unp.h"
void sig_chld(int signo);


int main(int argc,char *argv[])
{
	int listenfd,connfd;
	int socklen = 0;
	pid_t pid;
	struct sockaddr_in servaddr,cliaddr;
	
	if((listenfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		err_sys("socked error");
	
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	servaddr.sin_addr.s_addr = INADDR_ANY;
	socklen = sizeof(servaddr);
	if(bind(listenfd,(struct sockaddr *)&servaddr,socklen) < 0)
		err_sys("bind error");
	
	if(listen(listenfd,LISTENQ) < 0)
		err_sys("listen error");
	
	signal(SIGCHLD,sig_chld);
	while(1)
	{
		socklen = sizeof(cliaddr);
		if((connfd = accept(listenfd,(struct sockaddr *)&cliaddr,&socklen)) < 0) 
		{
			if(errno == EINTR)
			{
				continue;
			} else {
				err_sys("error get tcp connection!");
			}
			err_sys("accept error");
		}
		if((pid = fork()) == 0) {
			close(listenfd);
			str_echo(connfd);
			close(connfd);
			exit(0);	//ÍË³ö½ø³Ì
		}
		close(connfd);
	}
}

void str_echo(int sockfd)
{
	ssize_t n;
	char line[MAXLINE];
	
	for(;;)
	{
		if((n = Readline(sockfd,line,MAXLINE)) == 0)
			return;								/*connection closed by other end*/
		Fputs(line,stdout);
		Writen(sockfd,line,n);
	}
}

//¿¿¿¿¿¿¿¿
void sig_chld(int signo)
{
	pid_t pid;
	int stat;
	char str[100];
	while((pid = waitpid(-1,&stat,WNOHANG)) > 0){
		//printf("child %d has terminated",pid);
		snprintf(str,sizeof(str),"child %d has terminated\n",pid);
		perror(str);
	}
	return;
}
