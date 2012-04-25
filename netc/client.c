#include "unp.h"
#include "error.c"

void str_cli(FILE *fp,int sockfd);
int main(int argc,char *argv[])
{
	int sockfd;
	socklen_t socklen;
	struct sockaddr_in servaddr;
	
	if(argc != 2)
		err_quit("usage a.out <IP Address>");
	if((sockfd = socket(AF_INET,SOCK_STREAM,0)) < 0)
		err_sys("socket err");
		
	memset(&servaddr,0,sizeof(servaddr));	
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	socklen = sizeof(servaddr);
	if(connect(sockfd,(struct sockaddr *)&servaddr,socklen) < 0)
		err_sys("connect error");
	
	str_cli(stdin,sockfd);
	close(sockfd);
	return 0;
}

void str_cli(FILE *fp,int sockfd)
{
	char sendline[MAXLINE],recvline[MAXLINE];
	while(Fgets(sendline,MAXLINE,fp) != NULL) 
	{
		Writen(sockfd,sendline,strlen(sendline));
		if(Readline(sockfd,recvline,MAXLINE) == 0)
			err_quit("str_cli:server terminated prematurely!");
		Fputs(recvline,stdout);
	}
}
