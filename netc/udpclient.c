#include "unp.h"

int main(int argc,char *argv[])
{
	int sockfd;
	struct sockaddr_in servaddr;
	char line[MAXLINE];
	
	if(argc != 2)
		err_quit("usage:ucpcli <IPADDR>");
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	inet_pton(AF_INET,argv[1],&servaddr.sin_addr);
	
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);
	dg_cli(sockfd,(SA*)servaddr,sizeof(servaddr));
	exit(0);
}


void dg_cli(int sockfd,struct sockaddr *pservaddr,socklen_t socklen)
{
	int nread;
	char sendline[MAXLINE],recvline[MAXLINE];
	struct pollfd pfds[2];
	bzero(pollfd[0],sizeof(pollfd[0]));
	bzero(pollfd[0],sizeof(pollfd[0]));
	pfds[0].fd = fileno(stdin);
	pfds[0].event = POLLRDNORM;
	pfds[1].fd = sockfd;
	pfds[1].event = POLLRDNORM;
	for(;;)
	{
		nread = Poll(pfds,2,INFINITE);
		
		if(pfds[0].revent | POLLRDNORM)
		{
			Fgets(sendline,
		}
	}
}
