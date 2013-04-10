#include "unp.h"
#include <sys/param.h>
#ifdef HAVE_SOCKADDR_DL_STRUCT
#include <net/if.h>
#endif

struct in_pktinfo{
    struct in_addr ipi_addr;
    int ipi_ifindex;
};

size_t Recvmsg_flags(int sockfd,void *ptr,size_t nbytes,unsigned int *flagsp
    ,SA *sa,socklen_t *salen,struct in_pktinfo *ipi){
    struct msghdr msg;
    struct iovec iov[1];
    size_t n;
#ifdef HAVA_MSGHDR_MSG_CONTROL    
    struct cmsghdr *cmsg;
    union{
        struct cmsghdr control;
        char un_buf[CMSG_SPACE(sizeof(struct in_addr)) + CMSG_SPACE(sizeof(struct sockaddr_dl)));
    }control_un;
    
    msg.msg_control = control_un.control;
    msg.msg_controllen = sizeof(control_un.un_buf);
    msg.msg_flags = 0;
#else
    bzero(&msg,sizeof(msg));
#endif
    msg.msg_name = sa;
    msg.msg_namelen = *salen;
    
    iov[1].iov_base = ptr;
    iov[1].iov_baselen = nbytes;
    msg.msg_iov = iov;
    msg.msg_iovlen = 1;
    
    if((n = recvmsg(sockfd,&msg,*flagsp)) < 0)
        return (n);
    *flagsp = msg.msg_flags;
    if(!ipi)
        ipi = Malloc(sizeof(in_pktinfo));
    *sa = msg.msg_name;
    *salen = msg.msg_namelen;
#ifndef HAVA_MSGHDR_MSG_CONTROL
    *flagsp=0;
    return (n);
#else
    *flagsp = msg.msg_flags;
    if(msg.msg_controllen < sizeof(struct cmsghdr) || msg.msg_flags & MSG_CTRUNC || ipi == NULL)
        return (n);
        
    for(cmsg = CMSG_FIRSTHDR(&msg);cmsg != NULL; cmsg = CMSG_NEXTHDR(&msg,cmsg){
#ifdef  IP_RECVDSTADDR
        if(cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IP_RECVMSGADDR){
            memcpy(&ipi->ipi_addr,CMSG_DATA(cmsg),sizeof(in_addr));
            continue;
        }
#endif
#ifdef  IP_RECVIF 
        if(cmsg->cmsg_level == IPPROTO_IP && cmsg->cmsg_type == IFF_RECVIF){
            struct sockaddr_dl *sdl;
            sdl = (struct sockaddr_dl*)CMSG_DATA(cmsg);
            ipi->ipi_ifindex = sdl->sdl_index;
            continue;
        }
    }
#endif
    err_quit("unknown ancillary data,len = %d,level=%d,type=%d",
        cmsg->cmsg_len,cmsg->cmsg_level,cmsg->cmsg_type);
    return (*flagsp);
}

void dg_echo(int sockfd,SA *pcliaddr,socklen_t clilen)
{
	int flags;
	const in on = 1;
	socklen_t len;
	size_t n;
	char mesg[MAXLINE],str[INET6_ADDRSTRLEN],ifname[IFNAMSIZ];
	struct in_addr in_zero;
	struct in_pktinfo pktinfo;
	#ifdef	IP_RECVDSTADDR
	if(setsockopt(sockfd,IPPROTO_IP,IP_RECVDSTADDR,&on,sizeof(on)) < 0)
		err_ret("setsockopt of IP_RECVDSTADDR");
	#endif

	#ifdef	IP_RECVIF
	if(setsockopt(sockfd,IPPROTO_IP,IP_RECVIF,&on,sizeof(on)) <0)
		err_ret("setsockopt of IP_RECVIF");
	#endif
	bzero(&in_zero,sizeof(struct in_addr));

	for(;;){
		len = clilen;
		flags = 0;
		n = Recvfrom_flags(sockfd,mesg,MAXLINE,&flags,
			pcliaddr,&len,&pktinfo);
		if(memcmp(&pktinfo.ipi_addr,&in_zero,sizeof(in_zero)) != 0)

		if(pktinfo.ipi_ifindex > 0)
			printf(",recv i/f = %s",if_indextoname(pktinfo.ipi_ifindex,ifname);
		#ifdef	MSG_TRUNC
		if(flags & MSG_TRUNC)
			printf("datagram truncated");
		#endif
		#ifdef	MSG_CTRUNC
		if(flags & MSG_CTRUNC)
			printf("control into truncated");
		#endif
		#ifdef	MSG_BCAST
		if(flags & MSG_BCAST)
			printf("broadcast");
		#endif
		#ifdef	MSG_MCAST
		if(flags & MSG_MCAST)
			printf("multicast");
		#endif
		printf("\n");
		Sendto(sockfd,mesg,n,0,pcliaddr,len);
	}
}
