#include "unp.h"

struct addrinfo *host_serv(const char *hostname,const char *service,int family,int socktype)
{
	int n;
	struct addrinfo hint,*result;
	bzero(&hint,sizeof(addrinfo));

	hint.ai_flags = AI_CANONNAME;
	hint.ai_family=family;
	hint.ai_socktype=socktype;

	if((n = getaddrinfo(hostname,service,&hint,&result)) != 0)
		return (NULL);
	return (result);
}
