#include "unp.h"

int main(int argc,char *argv[])
{
	int	sockfd;
	if(argc != 3)
		err_quit("usage:tcpsend01 <host> <service>");
	sockfd = Tcp_connect(argv[1],argv[2]);
	Write(sockfd,"123",3);
	printf("write 3 bytes of normal data\n");
	sleep(1);

	send(sockfd,"4",1,MSG_OOB);
	printf("write 1 bytes of OOB data\n");
	sleep(1);

	Write(sockfd,"56",2);
	printf("write 2 bytes of normal data\n");
	sleep(1);

	Send(sockfd,"7",1,MSG_OOB);
	printf("wirete 1 bytes of OOB data\n");

	Write(sockfd,"89",2);
	printf("write 2 bytes of normal data\n");
	sleep(1);

	exit(0);
}
