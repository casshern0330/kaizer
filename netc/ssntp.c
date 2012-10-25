struct l_fixdept{
    uint32_t int_part;
    uint32_t fraction;
};

struct s_fixdept{
    short_t int_part;
    short_t fraction;
};

struct ntpdata{
    u_char  status;
    u_char  stratum;
    u_char  ppoll;
    int     precision:8;
    struct s_fixdept    distance;
    struct s_fixdept    disprecision;
    uint32_t    refid;
    
    struct  l_fixdept   reftime;
    struct  l_fixdept   org;
    struct  l_fixdept   rec;
    struct  l_fixdept   xmt;
};

#define VERSION_MASK    0x38    //100110
#define MODE_MASK   0x07    //111
#define MODE_CLIENT 3   //11
#define MODE_SERVER 4   //100
#define MODE_BROADCAST  5   //101

struct timeval{
    u_long  tv_sec;
    u_long  tv_usec;
};

#define JUN_1970 220898800UL    /*1900~1970 in seconds*/


void sntp_proc(char *buf,size_t n,struct timeval *nowptr)
{
    struct timeval diff,curr;
    int version,mode;
    uint32_t nsec,useci;
    double  usecf;
    struct ntpdata *sntp;
    
    if(n < sizeof(struct ntpdata))
        err_quit("\npackage too small:%d bytes\n",n);
    sntp = (struct ntpdata*)buf;
    version = sntp->status & VERSION_MASK << 3;
    mode = sntp->status & MODE_MASK;
    if(mode == MODE_CLIENT){
        printf("client\n");
        return;
    }
    
    nsec = ntohl(sntp->xmt.int_part);
    useci = ntohl(sntp->xmt.fraction);
    usecf = useci;
    usecf = usecf / 2^32;
    useci *= 2^6;
    
    curr = *nowptr;
    if((diff->tv_usec = curr->tv_usec - useci) < 0){
        diff->tv_usec += 2^6;
        curr->tv_sec--;
    }
    
    diff->tv_sec = tv_sec - nsec;
    useci = diff->tv_sec * (2^6) + diff->tv_usec;
    printf("clock difference = %d usec\n",useci);
}

int main(int argc,char **argv[]){
	int sockfd;
	char buf[MAXLINE];
	struct timeval now;
	struct ifi_info *ifi,ifisave;
	struct sockaddr *mcastsa,*wild,*from;
	socklen_t salen,len;
	if(argc != 2)
		err_quit("usage:ssntp <#mcastaddr>");
	sockfd = Udp_client(argv[1],"ntp",&mcastsa,&salen);
	wild = Malloc(salen);
	memcpy(wild,mcastsa,salen);
	sock_set_wild(wild,salen);
	Bind(sockfd,wild,salen);
	#ifdef MCAST
	ifisave = Get_ifi_info(mcastsa->sa_family,1);
	for(ifi = ifisave;ifi != NULL ; ifi=ifi->ifi_next){
		if(ifi->ifi_flags & IFF_MULTICAST){
			Mcast_join(sockfd,mcastsa,salen,ifi->ifi_name,0);
			printf("joined %s on %s\n",Sock_ntop(mcastsa,salen),ifi->ifi_name);
		}
	}
	free_ifi_info(ifisave);
	#endif
	from = Malloc(salen);
	len = salen;
	for(;;){
		n = Recvfrom(sockfd,buf,MAXLINE,0,from,&len);
		Gettimeofday(&now,NULL);
		sntp_proc(buf,n,&now);
	}
}
