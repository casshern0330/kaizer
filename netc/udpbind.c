#include "udpifi.h"

int mydg_echo(int fd,SA *sa,socklen_t salen,SA *myaddr);

int main(int argc,char *argv[])
{
	int sockfd;
	const int on = 1;
	pid_t pid;
	struct sockaddr_in *sa,cliaddr,wildaddr;
	struct ifi_info *ifi,*ifihead;

	for(ifi = ifihead = get_ifi_info(AF_INET,1);ifi != NULL;
		ifi=ifi->ifi_next){
		sa = (struct sockaddr_in*)ifi->ifi_addr;
		sa->sin_port = htons(SERV_PORT);
		sa->sin_family = AF_INET;
		sockfd = Socket(AF_INET,SOCK_DGRAM,0);
		setsockopt(sockfd,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
		Bind(sockfd,(SA*)sa,sizeof(*sa));
		if((pid = fork()) == 0){
			mydg_echo(sockfd,*cliaddr,sizeof(cliaddr),(SA*)sa);
			exit(0);
		}

		if(ifi->ifi_flags & IFF_BROADCAST){
			sockfd = Socket(AF_INET,SOCK_DAGRAM,0);
			setsockopt(sockfd,SOL_SOCKET,SOCK_DGRAM,&on,sizeof(on));
			sa = (struct sockaddr_in *)ifi->ifi_brdaddr;
			sa ->sin_port = htons(SERV_PORT);
			sa->sin_family = AF_INET;

			if(Bind(sockfd,sa,sizeof(*sa)) < 0){
				if(errno == EINUSEADDR){
					err_quit("EINUSERADDR:for %s\n",Sock_ntop(sa,sizeof(*sa));
				}else{
					err_quit("error bind for %s\n",Sock_ntop(sa,sizeof(*sa)));
				}
			}
			printf("bound from %s\n",Sock_ntop(sa,sizeof(*sa)));

			if((pid = fork()) == 0){
				mydg_echo(sockfd,*cliaddr,sizeof(cliaddr),(SA*)sa);
				exit(0);
			}
		}

		sockfd = Socket(AF_INET,SOCK_DGRAM,0);
		setsockopt(sockfd,SOL_SOCKET,SOCK_DGRAM,&on,sizeof(on));
		bzero(&wildaddr,sizeof(wildaddr));
		wildaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		wildaddr.sin_family = AF_INET;
		wildaddr.sin_port = htons(SERV_PORT);
		if((pid = fork()) == 0){
			mydg_echo(sockfd,*cliaddr,sizeof(cliaddr),(SA*)sa);
			exit(0);
		}
		exit(0);
	}
}


void mydg_echo(int sockfd,SA *cliaddr,socklen_t clilen,SA *myaddr)
{
	size_t n;
	char	mesg[MAXLINE];
	socklen_t len;
	for(;;){
		len = clilen;
		n = Recvfrom(sockfd,mesg,MAXLINE,0,cliaddr,&len);
		printf("child %d,datagram from %s",getpid(),Sock_ntop(cliaddr,len));
		printf("to %s\n",Sock_ntop(myaddr,len);
		Sendto(sockfd,mesg,MAXLINE,0,cliaddr,len);
	}
}
