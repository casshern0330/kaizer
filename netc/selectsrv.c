#include "unp.h"

int main(int argc,char *argv[])
{
	fd_set rset;
	int i;
	int nread,maxfd,imax;
	int listenfd,connfd,client[MAXSIZE];
	socklen_t socklen;
	struct sockaddr_in servaddr,cliaddr;
	char recvline[MAXLINE];
	FD_ZEOR(&rset);
	for(i=0;i<MAXSIZE;i++)
		client[i]=-1;
	
	listenfd = socket(AF_INET,SOCK_STREAM,0);
	maxfd = listenfd;
	memset(&servaddr,0,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8998);
	servaddr.sin_addr.s_addr = ANY_SOCKADDR;
	Bind(listenfd,servaddr,sizeof(servaddr));
	
	for(;;)
	{
		FD_SET(listenfd,&rset);
		if((nread = select(maxfd+1,&rset,NULL,NULL,NULL)) < 0)
			err_quit("select error");
		
		if(FD_ISSET(listenfd,&rset))
		{
			connfd = socket(AF_INET,SOCK_STREAM,0);
			for(i=0;i<MAXSIZE;i++)
				if(client[i] < 0)
				{
					client[i] = connfd;
					break;
				}
			if(i == MAXSIZE)
				err_quit("reach max client!!");
			if(connfd > maxfd)
				maxfd = connfd;
			if(i > imax)
				imax = i;
			
			if(--nread <= 0)
				continue;
		}
		
		for(i=0;i<=imax;i++) 
		{
			if(client[i] < 0)
				continue;
			if(FD_SET(client[i],&rset))
			{
				if(Readline(client[i],recvline,MAXLINE) == 0)
				{
					Close(client[i]);
					FD_CLR(client[i],&rset);
					client[i] = -1;
				}else
					Writen(client[i],recvline,strlen(recvline));
				
				if(--nread <= 0)
					break;
			}	
		}
	}
}
