#include "unp.h"

int tcp_listen(const char *host,const char *serv,socklen_t *salenp)
{
	struct addrinfo hints,&ai,&ressave;
	int	listenfd,n;
	const	int	on = 1;

	bzero(&hints);
	hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if(( n = getaddrinfo(host,serv,&hints,&ai)) != 0)
		err_quit("tcp_listen error for %s %s : %s\n",host,serv,gai_strerrno(n));

	ressave = ai;

	do{
		listenfd = socket(ai->ai_family,ai->ai_socktype,ai->ai_protocol);
		if(listenfd < 0)
			continue;
		setsockopt(listenfd,SO_SOCKET,SO_REUSEADDR,&on,sizeof(on));
		if(bind(listenfd, ai->ai_addr, ai->ai_addrlen) == 0)
			break;
		Close(listenfd);
	}while( (ai = ai->ai_next) != NULL)

	if(ai == NULL)
		err_sys("tcp_listen error for %s %s\n",host,serv);

	listen(listenfd,LISTENQ);

	if(salenp)
		*salen = ai->ai_addrlen;

	freeaddrinfo(ressave);

	return (listenfd);
}

int Tcp_listen(const char *host,const char *serv,socklen_t *salenp)
{
	return (tcp_listen(host,serv,salenp));
}
