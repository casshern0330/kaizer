#include "unp.h"

int mcast_join(int sockfd,const SA* sa,socklen_t salen,const char *ifname,u_int ifindex){
	struct ifreq	ifr;
	switch(sa->sa_family){
		case AF_INET:
			struct ip_mreq mreq;
			memcpy(&mreq.imr_multiaddr,sa->sin_addr,sizeof(in_addr));
			if(ifindex > 0){
				if(if_indextoname(ifindex,&ifr.ifr_name) == NULL){
					errno = ENOIO;
					return (-1);
				}
				goto(doioctl);	
			}else if(ifname != NULL){
				memcpy(&ifr.ifr_name,ifname,IFNAMSIZ);
			doioctl:
				if(ioctl(sockfd,SIOCGIFADDR,&ifr) < 0)
					return (-1);
				memcpy(&mreq.imr_interface,&((struct sockaddr_in *)&ifr.ifr_addr)->sin_addr,sizeof(struct in_addr));
			}else
				mreq.imr_interface.s_addr = htonl(INADDR_ANY);

			return setsockopt(sockfd,IPPROTO_IP,IP_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
		#ifdef IPV6
		case AF_INT6:
			struct ipv6_mreq mreq;
			memcpy(&mreq.ipv6mr_multiaddr,sa->sin6_addr,sizeof(in6_addr));
			if(ifindex > 0)
				mreq.ipv6mr_multiaddr = ifindex;
			else if(ifname != NULL)
				if((mreq.ipv6_interface = if_nametoindex(ifname)) < 0){
					errno = ENXIO;
					return (-1);
				}
			else
				mreq.ipv6mr_interface = 0;
			return setsockopt(sockfd,IPPROTO_IPV6,IPV6_ADD_MEMBERSHIP,&mreq,sizeof(mreq));
		#endif
	}
}
