#include "unp.h"

int main(int argc,char *argv[])
{
	int	connfd,stdno,maxfd,flag;
	char	sendmsg[MAXLINE],recvmsg[MAXLINE];
	fd_set	rset;
	maxfd = connfd = stdno = -1;
	flag = 1;
	if( argc == 2 )
		connfd = Tcp_connect(NULL,argv[1]);
	else if(argc == 3)
		connfd = Tcp_connect(argv[1],argv[2]);
	else
		err_quit("usage: client02 [<host>] <serv>\n");
	FD_ZERO(&rset);
	
	stdno = fileno(stdin);
	maxfd = max(stdno,connfd);
	while(1){
		FD_SET(connfd,&rset);
		if(flag){
			FD_SET(stdno,&rset);
		}

		if((select(maxfd+1,&rset,NULL,NULL,NULL)) < 0)
			err_quit("select error\n");

		if(FD_ISSET(stdno,&rset)) {
			if(Fgets(sendmsg,MAXLINE,stdin) == NULL){
				flag = 0;
				FD_CLR(stdno,&rset);
				shutdown(connfd,SHUT_WR);
				continue;
			}else{
				Writen(connfd,sendmsg,strlen(sendmsg));
			}
		}

		if(FD_ISSET(connfd,&rset)) {
			if(Readline(connfd,recvmsg,MAXLINE) == 0) {
				if(flag == 0){
					printf("client program over!\n");
					break;	//客户端程序结束
				}else{
					err_quit("server terminated prematurely");
				}
			} else {
				Fputs(recvmsg,stdout);
			}
		}
	}
	exit(0);
}
