#include "unp.h"

int udp_connect(const char *hostname,const char *serv,void **saptr,socklen_t *lenp)
{
	int connfd;
	int n;
	struct addrinfo hints,*res,*ressave;
	char *buff[MAXLINE];

	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_flags = AI_CANONNAME;
	hitns.ai_family = AF_UNSPCE;
	hitns.ai_socktype = SOCK_DGRAM;
	if((n = getaddrinfo(hostname,serv,&hints,&res)) != 0)
		err_quit("udp_client error getaddrinfo for %s,%s:%s",hostname,serv,gai_strerror(n));
	
	ressave = res;
	do{
		connfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol);
		if(connfd < 0)
			break;

		if(connect(connfd,res->ai_addr,res->ai_addrlen) == 0)
			break;

		Close(connfd);
	}while((res=res->ai_next) != NULL);
	
	if(res == NULL)
		err_quit("udp_client error for %s,%s\n",hostname,serv);
	
	*saptr = Malloc(res->ai_socklen);
	memcpy(*saptr,res->ai_addr,res->socklen);
	*lenp = res->ai_socklen;

	freeaddrinfo(ressave);
	return (connfd);
}
