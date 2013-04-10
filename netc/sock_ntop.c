#include "unp.h"
#ifdef HAVA_SOCKADDR_DL_STRUCT
#include	<net/if_dl.h>
#endif

static char *mysock_ntop(const struct sockaddr *sa,socklen_t salen)
{
	char portstr[8];
	static char str[128];	/*unix domain is largest*/
	
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

#ifdef HAVE_SOCKADDR_DL_STRUCT
		case AF_LINK:{
			struct sockaddr_dl	*dl = (struct sockaddr_dl *)sa;
			if(dl->sdl_nlen > 0)
				snprintf(str,sizeof(str),"%*s (index %d)",
					sdl->sdl_nlen,&sdl->sdl_data[0],sdl->sdl_index);
			else
				snprintf(str,sizeof(str),"AF_LINK,index = %d",sdl->sdl_index);
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

