#include"unproute.h"

#define IFI_NAME 8
#define IFI_HADDR 16

//ifi_info description a interface infomation
struct ifi_info{
    char ifi_name[IFI_NAME];
    u_char  ifi_haddr[IFI_HADDR];
    u_short ifi_hlen;
    int ifi_flags;
    short   ifi_myflags;
    struct sockaddr *ifi_addr;
    struct sockaddr *ifi_brdaddr;
    struct sockaddr *ifi_dstaddr;
    struct ifi_info *ifi_next;
};

void get_rtaddr(int flags,SA *sa,struct sockaddr **ifi_info);
char *net_rt_iflist(int family,int flags,size_t &len);

struct ifi_info* get_ifi_info(int family,int doaliases){
    int flags;
    size_t len;
    char *buf,*next,*lim;
    struct if_msghdr *ifm;
    struct ifa_msghdr *ifam;
    struct sockaddr *sa,rti_info[RTAX_MAX];
    struct ifi_info *ifi,*ifisave,*ifihead,**ifipnext;
    
    buf = net_rt_iflist(family,0,&len);
    ifihead = NULL;
    ifipnext = &ifihead;
    lim = buf + len;
    for(next = buf;next < lim;next += ifm->ifm_msglen){
        ifm = (struct if_msghdr*)next;
        if(ifm -> ifm_type == RTM_IFINFO){
            if(((flags = ifm->ifm_flags) & IF_UP) == 0)
                continue;   //ignore if interface not up
            sa = (struct sockaddr *)(ifm+1);
            get_rtaddr(ifm->ifm_addrs,sa,rti_info);
            if((sa = rti_info[RTAX_IFP]) != NULL){
                ifi = malloc(sizeof(struct ifi_info));
                *ifipnext = ifi;
                ifipnext = &ifi->ifi_next;
                ifi->ifi_flags = flags;
                if(sa->sa_family == AF_LINK){
                    sdl = (struct sockaddr_dl*)sa;
                    if(sdl->sdl_len > 0)
                        snprintf(ifi_name,IFI_NAME,"%*s",
                            sdl->sdl_nlen,&sdl->sdl_data[0]);
                    else
                        snprintf(ifi_name,IFI_NAME,"index:%d",
                            sdl->sdl_index);
                            
                    if((ifi->ifi_hlen = sdl->sdl_alen) > 0)
                        memcpy(ifi->ifi_haddr,LLADDR(sdl),min(IFI_HADDR,sdl->sdl_alen));
                }
            }
        }else if(ifm->ifm_type == RTM_NEWADDR){
            if(ifi->ifi_addr){
                if(doaliases == 0)
                    continue;   /*have a new address for existing interface*/
                ifsave = ifi;
                ifi = malloc(sizeof(struct ifi_info));
                *ifpnext = &ifi;
                ifpnext = &ifi->ifi_next;
                ifi->ifi_flags = ifisave->ifi_flags;
                ifi->ifi_hlen = ifisave->ifi_hlen;
                memcpy(ifi->ifi_name,ifisave->ifi_name,IFI_NAME);
                memcpy(ifi->ifi_haddr,ifisave->ifi_haddr,IFI_HADDR);
            }
            ifam = (struct ifa_msghdr*)next;
            sa = (struct sockaddr*)(ifam+1);
            get_rtaddr(ifam->ifam_addrs,sa,rti_info);
            
            if((sa = rti_info[RTAX_IFA]) == NULL){
                ifi->ifi_addr = malloc(sa->sa_len);
                memcpy(ifi->ifi_addr,sa,sa->sa_len);
            }
            
            if((flags & IFF_BROADCAST) && (sa = rti_info[RTAX_BRD]) != NULL){
                ifi->ifi_brdaddr = malloc(sa->sa_len);
                memcpy(ifi->ifi_brdaddr,sa,sa->sa_len);
            }
            
            if((flags & IFF_POINTTOPOINT) && (sa = rti_info[RTAX_BRD]) != NULL){
                ifi->ifi_dstaddr = malloc(sa->sa_len);
                memcpy(ifi->ifi_dstaddr,sa,sa->sa_len);
            }
        }else
            err_quit("unexpected message type %d\n",ifm->ifm_type);
    }
    free(buf);
    return (ifhead);
}


char *net_rt_iflist(int family,int flags,size_t *lenp){
    int mib[6];
    char *buf;
    
    mib[0]=CTL_NET;
    mib[1]=AF_ROUTE;
    mib[2]=0;
    mib[3]=family;
    mib[4]=NET_RT_IFLIST;
    mib[5]=flags;
    
    if(sysctl(mib,6,NULL,lenp,NULL,0) < 0)
        return (NULL);
    if(buf = malloc(*lenp) == NULL)
        return (NULL)
    if(sysctl(mib,6,buf,lenp,NULL,0) < 0){
        free(buf);
        return (NULL);
    }
    return (buf);   
}

#define ROUNDUP(a,size) (a) & ((size)-1) ? (a) | ((size)-1) : (a)
#define NEXT_SA(ap) (ap) = (SA*)((caddr_t*)(ap) + (ap)->sa_len ? ROUNDUP((ap),sizeof(u_long)) : sizeof(u_long)) 

void get_rtaddr(int flags,SA *sa,struct sockaddr **ifi_info){
    int i;
    for(i=0;i<RTAX_MAX;i++){
        if(flags & 1 << i){
            ifi_info[i] = sa;
            NEXT_SA(sa);
        }else
            ifi_info[i] = NULL;
    }
}
