#include "unp.h"

int main(int argc,char *argv[])
{
	int sockfd,n;
	struct hostent *hp;
	struct servent *sp;
	struct in_addr **is;
	struct in_sockaddr servaddr;

	if(argc != 3)
		err_quit("usage: daytimetcpcli <IP> <service>");
	
	if((hp=gethostbyname(argv[1])) == NULL)
		err_quit("error gethostname %s",argv[1]);
	
	if((sp=getservbyname(argv[2])) == NULL)
		err_quit("error getservbyname %s",argv[2]);

	is = (struct in_addr **)hp->h_addr_list
	sockfd = socket(AF_INET,SOCK_STREAM,0);
	while(*is != NULL){
		bzero(servaddr,sizeof(servaddr));
		servaddr.sin_family = AF_INET;
		servaddr.sin_port = s_port;
		memset(&servaddr.sin_addr,is,sizeof(struct in_addr));
	}
}
