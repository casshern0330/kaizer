#include "unp.h"

int main(int argc,char *argv[]){
	int listenfd,sockfd;
	pid_t pid;
	socklen_t len;
	struct sockaddr_un un;
	if(argc != 2)
		err_quit("usage server_un <pathname>");
	memset(&un,0,sizeof(un));
	un.sun_family = AF_LOCAL;
	strncpy(un.sun_path,argv[1],sizeof(argv[1]));
	sockfd = socket(AF_LOCAL,AF_STREAM,0);
	bind(listenfd,&un,SUN_LEN(&un));
	Listen(listenfd,LISTENQ);
	for(;;){
		if((sockfd=accept(listenfd,&un,&len))<0){
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}
		if((pid=fork()) == 0) {
			Close(listenfd);
			str_echo(sockfd);
			exit(0);
		}
		Close(sockfd);
	}
}
