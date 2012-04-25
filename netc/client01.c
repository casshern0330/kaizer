#include "unp.h"

void str_cli(FILE *fp,int sockfd);

int main(int argc,char *argv[])
{
	int	sockfd;
	if(argc == 2)
		sockfd = Tcp_connect(NULL,argv[1]);
	else if(argc == 3)
		sockfd = Tcp_connect(argv[1],argv[2]);
	else
		err_quit("usage:client01 [<hostname>] service\n");
	
	str_cli(stdin,sockfd);
	Close(sockfd);
	exit(0);
}

void str_cli(FILE *fp,int sockfd)
{
	int fd,n;
	char recvmsg[MAXLINE],sendmsg[MAXLINE];
	fd = sockfd;
	
	while(Fgets(sendmsg,MAXLINE,fp) != NULL){
		Writen(fd,sendmsg,strlen(sendmsg));
		if(Readline(fd,recvmsg,MAXLINE) == 0)
			err_quit("str_cli:server terminated prematured\n");
		Fputs(recvmsg,stdout);
	}
}
