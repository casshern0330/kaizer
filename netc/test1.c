#include "unp.h"

int main(int argc,char *argv[])
{
	int	sockfd;
	char	buf[MAXSIZE];
	struct sockaddr_in servaddr;

	sockfd = Socket(AF_INET,SOCK_STREAM,0);

	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(13); 
	if( Inet_ntop(AF_INET,argv[1],&servaddr.sin_addr) < 0 )
		err_sys("error input");
	Connect(sockfd,(SA *)&servaddr,sizeof(servaddr));
	
	while(n = Read(sockfd,buf,MAXSIZE) > 0) {
		buf[n] = 0;
		fputs(buf,STDOUT);
	}
	if( n < 0 )
		err_sys("read error");
	return 0;
}
