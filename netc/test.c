#include "unp.h"

int main(int argc,char *argv[])
{
    int connfd;
    int n,port;
    struct sockaddr_in servaddr;
    ssize_t socklen;
    char ip[64],buf[MAXLINE];

    if(argc != 3)
        err_sys("Usage: client <IP> <PORT>");
    else {
        ip = strncpy(ip,sizeof(ip),argv[1]);
        port = atoi(argv[2]);
    }
    memset(&servaddr,0x0,sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(port);
    #servaddr.sin_addr.s_addr = inet_addr(ip);
    #inet_ntop(AF_INET,ip,&servaddr.sin_addr);
    inet_pton(AF_INET,ip,&servaddr.sin_addr);

}
