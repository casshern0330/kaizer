#include "unp.h"

/*
signal occur must in recvfrom ,or it must be a dead cycle
*/
void dg_cli(FILE *fp,int sockfd,const SA *pservaddr,socklen_t servlen){
	int n;
	const in on = 1;
	char sendline[MAXLINE+1],recvline[MAXLINE+1];
	socklen_t len;
	struct sockaddr_in *preply_addr;
	preply_addr = malloc(sizeof(struct sockaddr_in));
	bzero(preply_addr);
	SetSockopt(sockfd,SOL_SOCKET,SO_BROADCAST,&on,sizeof(on));
	Signal(SIGALAM,recvfrom_alarm);
	len = servlen;
	while(Fgets(sendline,MAXLINE,fp) != NULL){
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,len);
		alarm(5);
		for(;;){
			len=servlen;
			n = recvfrom(sockfd,recvlien,MAXLINE,0,preply_addr,&len);
			if(n < 0){
				if(errno == EINTR)
					break;
				else
					err_quit("recvfrom error!");
			}else{
				recvfrom[n] = 0;
				printf("from %s : %s",inet_ntop(preply_addr->sin_family,preply_addr->sin_addr.s_addr,preply_addr->sin_len),recvfrom);
			}
		}
	}
}

static void recvfrom_alarm(int signo){
	return;
}


/*
signal block by pselect,and pselect is atom operation
*/
void dg_cli(FILE *fp,int sockfd,const SA* pservaddr,socklen_t servlen){
	int n;
	const int on = 1;
	char sendline[MAXLINE+1],recvline[MAXLINE+1];
	fd_set rset;
	socklen_t len;
	sigset_t sigset_empty,sigset_alrm;
	struct sockaddr_in *preply_addr;
	preply_addr = Malloc(sizeof(struct sockaddr_in));
	FD_ZERO(&rset);
	Sigemptyset(&sigset_empty);
	Sigemptyset(&sigset_alrm);
	Sigaddset(&sigset_alrm,SIGALRM);
	Signal(SIGALRM,recvfrom_alrm);
	while(Fgets(sendline,MAXLINE,fp) != NULL){
		len = servlen;
		sendto(sockfd,sendline,strlen(sendline),0,pservaddr,len);
		Sigprocmast(SIG_BLOCK,&sigset_alrm,NULL);
		alarm(5);
		for(;;){
			FD_SET(sockfd,&rset);
			n = pselect(sockfd+1,&rset,NULL,NULL,NULL,&sigset_empty);
			if(n < 0){
				if(errno == EINTR)
					break;
				else
					err_quit("pselect error!");
			}else if(n != 1){
				err_quit("pselect error %d\n",n);
			}
			n = recvfrom(sockfd,recvline,MAXLINE,0,preply_addr,&len);
			recvlien[n] = 0;
			printf("from %s:%s\n",Sock_ntop_host(preply_addr,len),recvfrom);
		}
	}
}

























