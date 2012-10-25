#include "unp.h"

void send_all(int sockfd,SA *sa,socklen_t salen);
void recv_all(int sockfd,salen);

//usage sendrecv <IP-multicast-addr> <port#>

int main(int argc,char *argv[])
{
	int recvfd,sendfd,n;
	const in on = 1;
	socklen_t salen;
	struct sockaddr *sarecv,*sasend;
	if(argc != 3)
		err_quit("usage: sendrecv <IP-multicast-addr> <port#>");
	sendfd = Udp_client(argv[1],argv[2],&sasend,&salen);
	sarecv = malloc(salen);
	memcpy(sarecv,sasend,salen);
	recvfd = Socket(sarecv->sa_family,SOCK_DGRAM,0);
	bind(recvfd,sarecv,salen);
	setscokopt(recvfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
	if(fork() == 0)
		recv_all(recvfd,salen);
	send_all(sendfd,sasend,salen);
}


#define SENDRATE 5

void send_all(int sockfd,SA *sa,socklen_t salen)
{
	static char buf[MAXLINE+1];
	struct utsname myname;
	if(uname(&myuname) < 0)
		err_sys("uname error!");
	snprintf(buf,MAXLINE,"%s,%d\n",myname.nodename,getpid());
	for(;;){
		sendto(sockfd,buf,strlen(buf),0,sa,salen);
		sleep(SENDRATE);
	}
}

void recv_all(int sockfd,socklen_t salen)
{
	static char buf[MAXLINE+1];
	struct sockaddr *sa;
	socklen_t len;
	int n;
	sa = Malloc(salen);
	for(;;){
		len = salen;
		n = recvfrom(sockfd,buf,MAXLINE,0,sa,&len);
		buf[n] = 0;
		printf("from %s : %s",Sock_ntop(sa,salen),buf);
	}
}
