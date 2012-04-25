#include "unp.h"

int	listenfd,accpetfd;
void sig_urg(int signo);

int main(int argc,char *argv[])
{
	int	n;
	char	buf[100];
	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],NULL);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],NULL);
	else
		err_quit("usage:tcprecv01 [<host>] <service>");
	
	acceptfd = Accept(listenfd,NULL,NULL);
	signal(SIGURG,sig_urg);
	fcntl(acceptfd,F_SETOWN,getpid());
	for(;;){
		if((n = Read(sockfd,buf,sizeof(buf)-1)) == 0){
			printf("received EOF\n");
			exit(0);
		}
		buf[n] = 0;
		printf("read %d normal bytes:%s\n",n,buf);
	}
}

void sig_urg(int signo)
{
	int n;
	char	buf[100];
	n = recv(acceptfd,buf,sizeof(buf)-1,MSG_OOB);
	buf[n] = 0;
	printf("read %d OOB bytes:%s\n",n,buf);
}
