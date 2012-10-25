#include "unp.h"

int main(int argc,char *argv[])
{
	int	sockfd;
	struct sockaddr_in servaddr,cliaddr;
	socklen_t len;
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	len = sizeof(servaddr);
	Bind(sockfd,(SA*)servaddr,sizeof(len));
	
	dg_echo(sockfd,(SA*)cliaddr,sizeof(cliaddr));
}

void dg_echo(int fd,struct sockaddr_in *pcliaddr,socklen_t socklen)
{
	socklen_t len;
	char line[MAXLINE];
	for(;;)
	{
		len = socklen;
		n = Recvfrom(fd,line,MAXLINE,0,pcliaddr,&len);
		Sendto(fd,line,strlen(line),0,pcliaddr,len);
	}
}
