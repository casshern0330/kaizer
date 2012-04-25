#include "unp.h"

int readable_timeo(int fd,int sec){
	fd_set rset;
	struct timeval tv;
	FD_ZERO(&rset);
	FD_SET(fd,&rset);
	tv.tv_sec=sec;
	tv.tv_usec=sec;
	return (select(fd+1,&rset,NULL,NULL,&tv));
}

void dg_cli(FILE *fp,int sockfd,const struct sockaddr *sa,socklent_t servlen){
	int n;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	int sec=5;
	while(Fgets(sendline,MAXLINE,fp) != NULL){
		Sendto(sockfd,sendline,strlen(sendline),0,sa,servlen);
		if(readable_timeo(sockfd,5) == 0)
			fprintf(stderr,"socket timeout!\n");
		else{
			n=recvfrom(sockfd,recvline,MAXLINE,0,NULL,NULL);
			recvline[n]='\0';
			Fputs(recvline,stdout);
		}
	}
}

void dg_cli_options(FILE *fp,int sockfd,const struct sockaddr *sa,socklen_t servlen){
	int n;
	char sendline[MAXLINE],recvline[MAXLINE+1];
	struct timeval tv;
	tv.tv_sec=5;
	tv.tv_usec=0;
	setsockopt(sockfd,SOL_SOCKET,SO_RECVTIMEO,&tv,sizeof(tv));
	while(Fgets(sendline,MAXLINE,fp) != NULL){
		sendo(sockfd,sendline,MAXLINE,0,sa,servlen);
		n = recvfrom(sockfd,recvline,MAXLINE,0,sa,servlen);
		if(n<0){
			if(errno == EWOULDBLOCK)
				fprintf(stderr,"socket timeout!\n");
			else
				err_sys("recvfrom error");
		}
		recvline[n]='\0';
		Fputs(recvline,stdout);
	}
}
