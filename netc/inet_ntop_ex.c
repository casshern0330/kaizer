#include "unp.h"

#define IPLEN 32

int main(int argc,char *argv[])
{
    char ip[IPLEN],dst[8];
    struct in_addr s;

    if(argc != 2)
        err_sys("Usage:inet_ntop_pton_ex <IP>");
    else
        strcpy(ip,argv[1]);

    inet_pton(AF_INET,ip,(void *)&s);
    printf("inet_pton: 0x%x\n",s.s_addr);
    
    inet_ntop(AF_INET,(void *)&s,dst,sizeof(dst));
    printf("inet_ntop is %s\n",dst);
    return 0;
}
