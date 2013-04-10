#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netdb.h>
#define MAXLINE 1024

static char *mysock_ntop(const struct sockaddr *sa,socklen_t salen);

int main(int argc,char *argv[])
{
	struct addrinfo hint,*sai,*ai;
	char hostname[MAXLINE];
	char servicename[16];
	char *str = NULL;
	strcpy(hostname,argv[1]);
	strcpy(servicename,argv[2]);
	printf("server name is %s\n",hostname);
	bzero(&hint,sizeof(hint));
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_INET;
	hint.ai_socktype = SOCK_STREAM;
	if(getaddrinfo(hostname,servicename,&hint,&ai) != 0)
		abort();
	sai = ai;
	while(ai != NULL) {
		if((str = mysock_ntop(ai->ai_addr,ai->ai_addrlen)) != NULL) {
			printf("server info is %s\n",str);
		}
		ai = ai->ai_next;
	}
	freeaddrinfo(sai);
	return 0;
}


static char *mysock_ntop(const struct sockaddr *sa,socklen_t salen)
{
	char portstr[8];
	static char str[128];	/*unix domain is largest*/
	printf("str=%s\n",str);
	switch(sa->sa_family) {
		case AF_INET:{
			struct sockaddr_in *sin = (struct sockaddr_in *)sa;
			if(inet_ntop(sin->sin_family,&sin->sin_addr,str,sizeof(str)) == NULL)
				return (NULL);
			if(ntohs(sin->sin_port) != 0) {
				snprintf(portstr,sizeof(portstr),":%d",ntohs(sin->sin_port));
				strcat(str,portstr);
			}
			return str;
		}

#ifdef	IPV6
		case AF_INET6:{
			struct sockaddr_in6 *sin6 = (struct sockaddr_in *)sa;
			str[0]='[';
			if(inet_ntop(sin6->sin6_family,&sin6->sin6_addr,str+1,sizeof(str)-1) == NULL)
				return (NULL);
			if(ntohs(sin6->sin6_port) != 0) {
				snprintf(portstr,sizeof(portstr),"]:%d",ntohs(sin6->sin6_port));
				strcat(str,portstr);
			}
			return str;
		}
#endif

#ifdef AF_UNIX
		case AF_UNIX:{
			struct sockaddr_un *unp = (struct sockaddr_un *)sa;
			if(unp->sun_path[0] == 0)
				strcpy(str,"(no pathname bound)");
			else
				snprintf(str,sizeof(str),"%s",unp->sun_path);
			return str;
		}
#endif
		default:
			snprintf(str,sizeof(str),"sock_ntop: unknown AF_xxx: %d,len %d",
					sa->sa_family,salen);
			return (str);
	}
	return (NULL);
}

