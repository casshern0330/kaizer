#this is a README file in netc

#this is use to test 'git rm'

struct sockaddr_in{
	uint8_t sin_len;
	sa_family_t sin_family;	//uint8_t
	uint16_t sin_port;
	struct in_addr sin_addr;
};

struct sockaddr_in sa;
sa.sin_addr.s_addr = inet_pton(AF_INET,"127.0.0.1");

struct in_addr{
	in_addr_t s_addr;	//typedef uint32_t in_addr_t;
};

struct sockaddr_in6{
	uint8_t sin6_len;
	uint8_t sin6_family;
	uint16_t sin6_port;
	struct in6_addr sin6_addr;
};

struct in6_addr{
	in6_addr_t s6_addr[16];	//typedef uint8_t in6_addr_t;
};


struct sockaddr{
	uint_t sa_len;
	sa_family_t sa_family;
	char sa_data[14];
};

struct hostent *gethostbyname(const char *hostname);
//return 非NULL == success,NULL == error

struct hostent{
	char *h_name;
	char **h_aliases;
	int h_addrtype;		//AF_INET | AF_INET6
	int h_length;		//4 | 16
	char **h_addr_list;
};

struct hostent *gethostbyaddr(const char *hostaddr,sszie_t addrlen,int family);
//return 非NULL == success,NULL == error;

struct utsname{
	char sysname[_UTS_NAMESIZE];
	char nodename[_UTS_NODESIZE];
	char release[_UTS_NAMESIZE];
	char version[_UTS_NAMESIZE];
	char machine[_UTS_NAMESIZE];
};

int uname(struct utsname *name);
//return 0 == success, 非0 == error

struct servent{
	char *s_name;
	char **s_aliases;
	uint16_t s_port;
	char *s_proto;
};

struct servent *getservbyname(const char *servname,const char *protoname);
//return 非NULL == success,NULL == error
//protoname == tcp || udp

struct servent{
	char *s_name;
	char **s_aliases;
	uint16_t s_port;
	char *s_proto;
};

struct hostent{
	char *h_name;
	char **h_aliases;
	int h_addrtype;
	int h_length;
	char **h_addr_list;
};

struct hostent *gethostbyname(const char *hostname);
struct hostent *gethostbyaddr(const char *servicename,ssize_t addrlen,int family);

res_init();
_res.options |= RES_USE_INET6

struct addrinfo{
	int ai_flags;	//AI_PASSIVE | AI_CANONNAME
	int ai_family;	//AF_INET | AF_INET6
	int ai_socktype;	//SOCK_STREAM | SOCK_DGRAM
	int ai_protocol;	//IPPROTO_IPV4 | IPPROTO_IPV6
	size_t ai_addrlen;	//length of ai_len;
	char *ai_canonname;
	struct sockaddr *ai_addr;
	struct addrinfo *next;
};

int getaddrinfo(const char *hostname,const char *servicename,const struct addrinfo *hint,struct addrinfo **result);
//return 0 == success,非0 == error


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

<img src="why-vi-img/why-vi-1-1.gif" alt=""/>

bool equiv = equialent(entry.key(),qk.ley);
if(!entry.used && equialent(entry.key(),qk.ley) && (curcontext & entry.contexts)


struct ifconf{
	char ifc_name[IFNAMSIZ];
	union{
		haddr_t ifcu_haddr[IFHADDR];
		struct ifreq *ifcu_req;
	}ifc_ifcu;
};


struct ifreq{
	char ifr_name[IFNAMSIZ];
	union{
		struct sockaddr *ifru_addr;	//SIOCGIFCONF
		struct sockaddr *ifru_brdaddr;  //SIOCGBRDADDR
		struct sockaddr *ifru_dstaddr;	//SIOCGDSTADDR
		int flags			//SIOCGIFFLAGS
	}ifr_ifru;
};



struct sockaddr_dl{
	uint8_t	sdl_len;
	sa_family_t sdl_family;
	uint16_t sdl_type;	/RTM_GET | RTM_ADD | RTM_DEL | RTM_CHANGE
	uint8_t	sdl_index;
	uint8_t	sdl_nlen;
	uint8_t sdl_slen;
	uint8_t sdl_alen;
	char	sdl_data[12];
}

struct rt_msghdr{
	uint8_t	rtm_msglen;
	u_char	rtm_version;
	u_char	rtm_type;
	u_short rtm_index;
	int	rtm_flags;
	int	rtm_addrs;
	pid_t	rtm_pid;
	int	rtm_seq;
	int	rtm_errno;
	int	rtm_ints;
};




int sysctl(int *name,size_t namelen,void *oldp,size_t *oldplen,void *newp,size_t newplen);
//return  0 == success, -1 == error

struct msghdr{
	char *msg_name;
	size_t msg_namelen;
	struct cmsghdr *msg_control;
	size_t msg_controllen;
	struct iovec *msg_iov;
	size_t msg_iovlen;
};

struct cmsghdr{
	socklen_t cmsg_len;
	int	cmsg_level;	//IPPROTO_IP	|	IPPROTO_IPV6
	int	cmsg_type;	//IP_RECVMSGADDR	|	IP_RECVADDR
	char	*cmsg_data;
};

CMSG_SPACE(sizeof(data));
CMSG_DATA(*cmsghdr);
CMSG_FIRSTHDR(struct msghdr *msg);
CMSG_NEXTHDR(struct msghdr *msg,struct cmsgdhr *cmsg);


struct in_pktinfo{
	struct in_addr ipi_addr;
	int ipi_ifindex;
};

struct in6_pktinfo{
	struct in6_addr ipi6_addr;
	int	ipi6_ifindex;
};


struct rtt_info{
	float	rtt_rtt;
	float	rtt_srtt;	//smoothed RTT
	float	rtt_srttval;	//smothed mean deviation
	float	rtt_rto;
	int	rtt_nrexmt;
	uint32_t	rtt_base;	//#sec since 1/1/1970
};





























