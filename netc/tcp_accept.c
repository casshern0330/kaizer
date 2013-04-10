#include "unp.h"

int tcp_listen(const char *hostname,const char *service)
{
	int n,sockfd,listenfd;
	struct addrinfo hints,*res,*ressave;

	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if((n = getaddrinfo(hostname,service,&hints,&res)) != 0)
		err_quit("tcp_accept error getaddrinfo for %s,%s:%s",hostname,service,gai_strerror(n));
	
	ressave = res;
	do{
		if((listenfd = socket(res->ai_family,res->ai_socktype,res->ai_protocol)) < 0)
			continue;
		if(bind(listenfd,res->ai_addr,res->ai_addrlen) == 0)
			break;
		Close(listenfd);
	}while((res = res->next) != NULL);

	if(res == NULL)
		err_quit("tcp_listen error for %s,%s",hostname,service);
	Listenfd(listenfd,LISTENQ);

	if(addrlenp)
		*addrlenp = res->ai_socklen;
	freeaddrinfo(ressave);
	return (listenfd);
}
