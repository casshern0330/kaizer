#include "unp.h"

#define ROUNDUP(a,size) ((ap) & ((size)-1) ? (1 + (ap) | ((size)-1)) : (ap))
#define NETX_SA(ap) ((ap) = (SA *)((caddr_t)(ap) + ((ap)->sa_len ? ROUNDUP((ap),sizeof(u_long)) :sizeof(u_long)))

void get_rtaddr(int flags,struct sockaddr *sa,struct sockaddr **rt_info)
{
	int i;
	for(i=0 ; i < RTAX_MAX; i++){
		if( flags & (1 << i)){
			rt_info[i] = sa;
			NEXT_SA(sa);
		}else{
			rt_info[i] = NULL;
		}
	}
}


char *sock_mask(SA *sa,socklen_t salen)
{
	static char str[INET6_ADDRSTRLEN];
	unsigned char *ptr = sa->sa_data[2];

	if(sa -> sa_len == 0)
		return ("0.0.0.0");
	else if(sa->sa_len == 5)
		snprintf(str,INET6_ADDRSTRLEN,"%d.0.0.0",*ptr);
	else if(sa->sa_len == 6)
		snprintf(str,INET6_ADDRSTRLEN,"%d.%d.0.0",*ptr,*(ptr+1));
	else if(sa->sa_len == 7)
		snprintf(str,INET6_ADDRSTRLEN,"%d.%d.%d.0",*ptr,*(ptr+1),*(ptr+2));
	else if(sa->sa_len == 8)
		snprintf(str,INET6_ADDRSTRLEN,"%d.%d.%d.%d",*ptr,*(ptr+1),*(ptr+2),*(ptr+3));
	else
		snprintf(str,INET6_ADDRSTRLEN,"(unknown mask,len = %d,family = %d",sa->sa_len,sa->sa_family);
	return (str);
}

#define MAX_RT_INFO 8

#define BUFLEN (sizeof(struct rt_msghdr) + 512)

int main(int argc,char *argv[]){
	int fd,i,n;
	pid_t pid;
	struct sockaddr *sa,*rt_info[MAX_RT_INFO];
	struct rt_msghdr *rtm;
	char *buf;
	fd = Socket(AF_ROUTE,SOCK_RAW,0);
	buf = Malloc(BUFLEN);
	rtm = (struct rt_msghdr *)buf;
	sa = (struct sockaddr_in *)(rtm+1);
	rtm->rtm_msglen = sizeof(struct rt_msghdr) + sizeof(struct sockaddr);
	rtm->rtm_version = RTM_VERSION;
	rtm->rtm_type = RTM_GET;
	rtm->rtm_addrs = RTA_DST;
	rtm->rtm_pid = pid = getpid();
	rtm->rtm_seq = SEQ;
	sa->sin_family = AF_INET;
	inet_pton(AF_INET,argv[1],&sin->sin_addr);

	Write(fd,rtm,rtm->rtm_msglen);
	do{
		n = Read(fd,rtm,BUFLEN);
	}while(rtm->rtm_seq != SEQ || rtm->rtm_pid != pid || rtm->rtm_type != RTM_GET);
	rtm = (struct rt_msghdr*)buf;
	sa = (struct sockaddr*)(buf+1);
	get_rtaddr(rtm->rtm_addrs,sa,rt_info);
	if(sa = rt_info[RTAX_DST] != NULL)
		printf("dst:%s\n",Sock_ntop_host(sa,sa->sa_len);
	if(sa = rt_info[RTAX_GATEWAY] != NULL)
		printf("gateway:%s\n",Sock_ntop_host(sa,sa->sa_len);
	if(sa = rt_info[RTAX_NETMASK] != NULL)
		printf("netmask:%s\n",Sock_netmask(sa,sa->sa_len);
	if(sa = rt_info[RTAX_GENMASK] != NULL)
		printf("genmask:%s\n",Sock_netmask(sa,sa->sa_len);
	return (0);
}
