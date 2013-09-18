#include "unp.h"

#define IPLEN 32

int main(int argc,char *argv[])
{
    struct in_addr s;
    char ip[IPLEN],dst[IPLEN];

    if(argc != 2)
        sys_err("Usage:inet_ntoa <IP>");
    else
        strcpy(ip,argv[1]);

    inet_aton(ip,&s);
    printf("inet_aton is 0x%x\n",s->s_addr);
    printf("inet_ntoa is %s\n",inet_ntoa(&s));
}
