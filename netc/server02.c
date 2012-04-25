#include "unp.h"

#define MAXSIZE 20

int main(int argc,char *argv[])
{
	int	listenfd,acceptfd,maxfd,client[MAXSIZE],nread,i,imax;
	char	recvmsg[MAXLINE];
	struct sockaddr sa;
	socklen_t	salen;
	fd_set	rset,rsetsave;

	for(i=0;i<MAXSIZE;i++)
		client[i] = -1;
	listenfd = acceptfd = -1;
	imax = 0;

	if(argc == 2)
		listenfd = Tcp_listen(NULL,argv[1],&salen);
	else if(argc == 3)
		listenfd = Tcp_listen(argv[1],argv[2],&salen);
	else
		err_quit("usage : server02 [<host>] <serv>");

	maxfd = listenfd;
	FD_ZERO(&rsetsave);
	
	FD_SET(listenfd,&rsetsave);
	while(1){
		rset = rsetsave;
		if((nread = select(maxfd+1,&rset,NULL,NULL,NULL)) < 0)
			err_quit("select error");

		if(FD_ISSET(listenfd,&rset)) {
			if((acceptfd = accept(listenfd,&sa,&salen)) < 0) {
				if((errno == EINTR))
					continue;
				else
					err_sys("server02 error in accept!");
			}else {
				for(i=0;i<MAXSIZE;i++){
					if(client[i] < 0){
						client[i] = acceptfd;
						break;
					}
				}

				if(i == MAXSIZE)
					err_quit("reach max client!please wait\n");
				FD_SET(acceptfd,&rsetsave);
				if(acceptfd > maxfd)
					maxfd = acceptfd;
				if(i > imax)
					imax = i;
				if(--nread <= 0)
					continue;
			}
		}

		for(i=0;i<=imax;i++){
			if(client[i] < 0)
				continue;
			if(FD_ISSET(client[i],&rset)) {
				if((Readline(client[i],recvmsg,MAXLINE)) == 0){
					Close(client[i]);
					FD_CLR(client[i],&rsetsave);
					client[i] = -1;
					printf("has disconnect with one socket!\n");
				}else {
					Writen(client[i],recvmsg,strlen(recvmsg));
				}

				if(--nread <= 0)
					continue;
			}
		}
	}
}
