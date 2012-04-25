#include "unp.h"

int listenfd,acceptfd;
void sig_urg(int signo);

int main(int argc,char *argv[])
{
	char buf[100];
	int n,justreadoob=0;
	fd_set rset,xset;
	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],NULL);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],NULL);
	else
		err_quit("usgae:tcprecv02 [<host>] <service>");
	acceptfd = Tcp_accept(listenfd,NULL,NULL);
	FD_ZERO(&rset);
	FD_ZERO(&xset);
	for(;;){
		FD_SET(acceptfd,&rset);
		if(justreadoob)
			FD_SET(acceptfd,&xset);
		n = select(acceptfd+1,&rset,NULL,&xset,NULL);
		if(FD_ISSET(acceptfd,&rset)){
			if((n = Read(acceptfd,buf,sizeof(buf)-1)) == 0){
				printf("read data EOF\n");
				break;
			}
			buf[n] = 0;
			justreadoob = 1;
			printf("read %d bytes normal data:%s\n",n,buf);
		}

		if(FD_ISSET(accept,&xset)){
			if((n = Recv(acceptfd,buf,sizeof(buf)-1,MSG_OOB)) <= 0)
				err_quit("tcp recv oob data error,error no is %d\n",errno);
			buf[n] = 0;
			justreadoob = 1;
			printf("read %d bytes OOB data:%s\n",n,buf);
		}
	}
	exit(0);
}
