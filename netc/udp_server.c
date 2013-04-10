#include "unp.h"

int
udp_server(const char *host,const char *serv,socklen_t *lenp)
{
	int	sockfd,n;
	struct	addrinfo hints,*res,*ressave;

	bzero(&hints,sizeof(struct addrinfo));
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if( (n = getaddrinfo(host,serv,&hints,&res)) != 0)
		err_quit("udp_server error for %s %s : %s\n",host,serv,gai_strerror(n));

	ressave = res;

	do{
		sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
		if( sockfd < 0 )
			continue;

		if(bind(sockfd, res->ai_addr,res->ai_addrlen) == 0)
			break;
		Close(sockfd);
	}while( ( res = res->ai_next) != NULL);

	if(res == NULL)
		err_sys("udp_server error for %s %s\n",host,serv);

	if(lenp)
		*lenp = res->ai_addrlen;

	freeaddrinfo(ressave);

	return (sockfd);
}

int
Udp_server(const char *host,const char *serv,socklen_t *lenp)
{
	return (udp_server(host,serv,lenp));
}
