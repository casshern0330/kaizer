#include "unp.h"

#define IFI_NAME 16
#define IFI_HADDR 8

struct ifi_info{
	char ifi_name[IFI_NAME];
	u_char ifi_haddr[IFI_HADDR];
	u_short ifi_hlen;
	short ifi_flags;
	short ifi_myflags;
	struct sockaddr *ifi_addr;
	struct sockaddr *ifi_brdaddr;
	struct sockaddr *ifi_dstaddr;
	struct ifi_info *ifi_next;
}

void free_ifi_info(struct ifi_info *ifihead){
	struct ifi_info *ifi,*ifinext;
	
	for(ifi= ifihead;ifi!= NULL;ifi= ifinext){
		if(ifi->ifi_addr != NULL)
			free(ifi->ifi_addr);
		if(ifi->ifi_brdaddr != NULL)
			free(ifi->ifi_brdaddr);
		if(ifi->ifi_dstaddr != NULL)
			free(ifi->ifi_dstaddr);
		ifinext = ifi->ifi_next;
		free(ifi);
	}
}

struct ifi_info *ifi_info(int family,int doaliases){
	struct infi_into *ifi,*ifisave,*ifhead,**ifipnext;
	struct ifconf ifc;
	int flags,i,sockfd;
	sockfd = Socket(AF_INET,SOCK_DGRAM,0);

}

struct ifi_info *get_ifi_info(int family,int doaliases){
	struct ifi_info *ifi,*ifisave,*ifihead,**ifipnext;
	struct ifconf ifc;
	struct ifreq *ifr,ifcopy;
	struct sockaddr_in *sptr;
	char *buf,lastname[IFNAMSIZ],*ptr,*cptr;
	size_t len,salen;
	int i,sockfd,flags,myflags;
	sockfd = socket(family,SOCK_DGRAM,0);
	len = BUFLEN;
	buf = malloc(len);
	for(;;){
		ifc.ifc_len = len;
		ifc.ifc_buf = buf;
		if(ioctl(sockfd,SIOCGIFCONF,&ifc)<0)
			err_quit("ioctl error!");
		if(len == ifc.ifc_len)
			break;
		len += 10*sizeof(struct ifreq);
	}

	ifihead = NULL;
	ifipnext = &ifihead;
	lastname[0] = '\0';
	for(ptr = buf;ptr < buf + len;){
		ifr=(struct ifreq*)ptr;
		switch(ifr->ifr_addr.sa_family){
			case AF_INET:
				salen = sizeof(struct sockaddr_in);	
				break;
			case AF_INET6:
				salen = sizeof(struct sockaddr_in6);
				break;
			default:
				break;
		}
		ptr += salen + sizeof(ifr->ifr_name);
		if(cptr = strchr(ifr->ifr_name,':') != NULL)
			*cptr = 0;
		myflags = 0;
		if(strncmp(lastname,ifr->ifr_name,IFNAMSIZ) == 0){
			if(doaliases == 0)
				continue;
			myflags = IFF_ALIASE;
		}
		memcpy(lastname,ifr->ifr_name,IFNAMSIZ);
		ifcopy = *ifr;
		flags = ioctl(sockfd,SIOCGIFFLAGS,&ifcopy);
		if(flags & IFF_UP)
			continue;
		ifi = malloc(sizeof(struct ifi_info));
		*ifipnext = ifi;
		ifipnext = &ifi->ifi_next;
		ifi->ifi_flags = flags;
		ifi->ifi_myflags = myflags;
		memcpy(ifi->ifiname,ifr->ifr_name,IFNAMSIZ);
		if(ifi->ifi_addr == NULL){
			sptr = ifr->ifr_addr;
			ifi->ifi_addr = malloc(sizeof(struct sockaddr_in));
			memcpy(ifi->ifi_addr,ifr->ifr_addr,IFNAMSIZ);
			#ifdef SIOCGIFBRDADDR
			if(flags & IFF_BROADCAST){
				ioctl(sockfd,SIOCGIFBRDADDR,&ifcopy);
				sptr = (struct sockaddr_in*)ifcopy.ifr_brdaddr;
				ifi->ifi_brdaddr = malloc(sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_brdaddr,sptr,sizeof(struct sockaddr_in));
			}
			#endif
			#ifdef SIOCGIFDSTADDR
			if(flags & IFF_BROADCAST){
				ioctl(sockfd,SIOCGIFBRDADDR,&ifcopy);
				sptr = (struct sockaddr_in*)ifcopy.ifr_brdaddr;
				ifi->ifi_brdaddr = malloc(sizeof(struct sockaddr_in));
				memcpy(ifi->ifi_brdaddr,sptr,sizeof(struct sockaddr_in));
			#endif
		}
	}
	return (ifihead);
}
