#include "unp.h"

int main(int argc,char *argv[])
{
	struct addrinfo hint,*sai;
	char str[MAXLINE];
	const char *servname = "localhost";
	bzero(&hint,sizeof(hint));
	hint.ai_family = AF_INET;
	if(getaddrinfo("www.baidu.com","http",NULL,&sai) != 0)
		err_quit("error get addrinfo\t%s\n",servname);

	while(sai != NULL)
	{
		printf("server ip is %s\n",inet_ntop(sai->ai_family,sai->ai_addr,str,sizeof(str)));
		sai = sai->ai_next;
	}	
	return 0;
}
