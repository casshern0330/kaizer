#include "unpthread.h"

static void copyto(void);

static FILE *fp;
static int sockfd;

void str_cli(FILE *fp_arg,int sockfd_arg)
{
	char	recvline[MAXLINE];
	pthread_t tid;
	sockfd = sockfd_arg;
	fp = fp_arg;
	Pthread_create(&tid,NULL,copyto,NULL);
	while(Readline(sockfd,recvline,MAXLINE) != NULL)
		Fputs(recvline,stdout);
}

static void copyto(void)
{
	char	sendline[MAXLINE];
	while(Fgets(sendline,MAXLINE,fp) != NULL)
		Writen(sockfd,sendline,strlen(sendline));
	shutdown(sockfd,SHUT_WR);		//EOF on stdin,send FIN
	return (NULL);
}
