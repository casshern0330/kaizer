#include "unp.h"

#include <netinet/in_system.h>
#include <netinet/ip.h>

#define	SRCRTLEN	44

static u_char	*optr;
static u_char	*lenptr;
static int	ocnt;

u_char *inet_srcrt_init(void)
{
	optr = Malloc(SRCRTLEN);
	bzero(optr,SRCRTLEN);
	ocnt = 0;
	return (optr);
}


int	inet_srcrt_add(char *hostptr,int type)
{
	int	len;
	struct	addrinfo *ai;
	struct	sockaddr_in *sin;

	if(ocnt > 9)
		err_quit("too many source routes with :%s\n",hostptr);

	if(ocnt == 0){
		*optr++ = IPOPT_NOP;
		*opt++ = type ? IPOPT_SSRR : IPOPT_LSRR;
		lenptr = optr++;
		*optr++ = 4;
	}

	ai = Host_serv(hostptr,"",AF_INET,0);
	sin = (struct sockaddr_in *)ai->ai_addr;

	memcyp(optr,sin->sin_addr,sizeof(struct in_addr));
	optr += sizeof(struct in_addr);
	ocnt++;
	len = 3 + sizeof(ocnt * sizeof(struct in_addr));
	*lenptr = len;
	return (len + 1);	//size for setsockopt
}
