#include "unp.h"


static char name[128];
char* mysock_ntop(const struct sockaddr *sa,socklen_t salen);

int main(int argc,char *argv[])
{
	struct addrinfo hints,*ai,*resai;
	int n;
	char *servicename,*hostname;
	servicename = NULL;
	hostname = NULL;
	if(argc == 2){
		servicename = argv[1];
	}else if(argc == 3){
		servicename = argv[2];
		hostname = argv[1];
	}else 
		err_quit("usgae:print_addr [<Hostname>] servicename");

	bzero(&hints,sizeof(struct addrinfo));
	//hints.ai_flags = AI_PASSIVE;
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_DGRAM;

	if((n = getaddrinfo(hostname,servicename,&hints,&ai)) != 0)
		err_sys("error getaddrinfo for %s %s:%s\n",hostname,servicename,gai_strerror(n));

	resai = ai;
	do{
		//printf("server addr is %s\n",Sock_ntop(ai->ai_addr,ai->ai_addrlen));
		printf("server addr is %s\n",mysock_ntop(ai->ai_addr,ai->ai_addrlen));
	}while((ai = ai->ai_next) != NULL);

	freeaddrinfo(resai);
	exit(0);
}

char* mysock_ntop(const struct sockaddr *sa,socklen_t salen)
{
	struct sockaddr_in *sin;
	char port[20];
	sin = (struct sockaddr_in *)sa;
	snprintf(port,sizeof(port),":%d",ntohs(sin->sin_port));
	if(inet_ntop(sin->sin_family,&sin->sin_addr,name,sizeof(name)) == NULL)
		return (NULL);
	strcat(name,port);
	return name;
}
