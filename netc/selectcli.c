#include "unp.h"

//使用select函数处理str_cli函数

void str_cli(FILE *file,int sockfd)
{
	fd_set rset;
	int nread,maxfd,flag;
	char sendline[MAXLINE],recvline[MAXLINE];
	flag = 1;
	
	FD_ZERO(&rset);
	maxfd = max(fileno(file),sockfd);
	for(;;)
	{
		FD_SET(sockfd,&rset);
		if(flag)
			FD_SET(fileno(file),&rset);
		if(select(maxfd+1,&rset,NULL,NULL,NULL) <0)
			err_quit("select error!");
		
		if(FD_ISSET(fileno(file),&rset)
		{
			if(Fgets(sendline,MAXLINE,file) == NULL)
			{
				flag = 0;
				shutdown(sockfd,SHUT_WR);
				FD_CLR(fileno(file),&rset);
				continue;
			}else
				Writen(sockfd,sendline,strlen(buffer));
		}
		
		if(FD_ISSET(sockfd,&rset))
		{
			if(Readline(sockfd,recvline,MAXLINE) == 0)
			{
				if(flag == 0)
					break;
				else
					err_quit("str_cli:server terminated prematurely");
			}
			Fputs(recvline,file);
		}
	}

void str_cli(FILE *file,int sockfd)
{
	fd_set rset;
	int nread,maxfd,flag;
	char sendline[MAXLINE],recvline[MAXLINE];
	flag = 1;
	
	FD_ZERO(&rset);
	maxfd = max(fileno(file),sockfd);
	for(;;)
	{
		FD_SET(sockfd,&rset);
		if(flag)
			FD_SET(fileno(file),&rset);
		if(select(maxfd+1,&rset,NULL,NULL,NULL) <0)
			err_quit("select error!");
		
		if(FD_ISSET(fileno(file),&rset)
		{
			if(Fgets(sendline,MAXLINE,file) == NULL)
			{
				flag = 0;
				shutdown(sockfd,SHUT_WR);
				FD_CLR(fileno(file),&rset);
				continue;
			}else
				Writen(sockfd,sendline,strlen(buffer));
		}
		
		if(FD_ISSET(sockfd,&rset))
		{
			if(Readline(sockfd,recvline,MAXLINE) == 0)
			{
				if(flag == 0)
					break;
				else
					err_quit("str_cli:server terminated prematurely");
			}
			Fputs(recvline,file);
		}
	}
}
