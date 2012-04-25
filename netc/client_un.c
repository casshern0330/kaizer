#include "unp.h"

int main(int argc,char *argv[]){
	int sockfd;
	struct sockaddr_un un;
	if(argv != 2)
		err_quit("usage:client_un <pathname>");
	sockfd = socket(AF_LOCAL,SOCK_STREAM,0);
	bzero(&un,0);
	un.sun_family = AF_LOCAL;
	strncpy(un.sun_path,argv[1],sizeof(argv[1]));
	Connect(sockfd,(struct sockaddr*)&un,sizeof(un));
	str_cli(stdin,sockfd);
	exit(0);
}
