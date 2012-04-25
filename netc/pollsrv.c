#include "unp.h"
#include <limit.h>

int main(int argc,char *argv[])
{
	int maxi,i,nread;
	int listenfd,connfd;
	char line[MAXLINE];
	ssize_t n;
	socklen_t socklen;
	struct pollfd client[OPEN_MAX];
	struct sockaddr_in servaddr,cliaddr;
	
	listenfd = Socket(AF_INET,SOCK_STREAM,0);
	maxfd = listenfd;
	bzero(&servaddr,sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(8998);
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	Bind(listenfd,(SA *)&servaddr,sizeof(servaddr));
	client[0].fd = listenfd;
	client[0].fd = POLLRDNORM;
	for(i=1;i<OPEN_MAX;i++)
		client[i].fd = -1;
	maxi = 0;
	
	for(;;){
		nread = Poll(client,maxi+1,INFTIM);
		
		if(client[0].revent & POLLRDNORM) 
		{
			socklen = sizeof(cliaddr);
			connfd = accept(listenfd,(SA*)cliaddr,&socklen);
			
			for(i=0;i<OPEN_MAX;i++)
			{
				if(client[i].fd < 0){
					client[i].fd = connfd;
					client[i].event = POLLRDNORM;
					break;
				}
			}
			
			if(i == OPEN_MAX)
				err_quit("too many clients");
				
			if(i > maxi)
				maxi = i;
			if(--nread <= 0)
				continue;
		}
		
		for(i=0;i<maxi;i++)
		{
			if(client[i].fd < 0)
				continue;
			if(client[i].revent & POLLRDNORM)
			{
				if((n = Readline(client[i],line,MAXLINE)) < 0)
				{
					if(errno == ECONNRESET)
					{
						Close(client[i]);
						client[i].fd = -1;
					} else
						err_sys("readline error!");
				}
				else if(n == 0)
				{
					Close(client[i].fd);
					client[i].fd = -1;
				} else 
					Writen(client[i],line,n);
			}
			
			if(--nread <= 0)
				break;
		}
	}
