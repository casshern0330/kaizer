#include "unp.h"
#include <netinet/in_systm.h>
#include <netinet/ip.h>

int inet_srcrt_print(u_char *ptr,int len)
{
	u_char	c;
	char	str[INET_ADDRSTRLEN];

	struct in_addr	hop1;
	bzero(&hop1,sizeof(struct in_addr));

	memcpy(&hop1,ptr,sizeof(struct in_addr));

	*ptr += sizeof(struct in_addr);

	while(((c = *ptr++) != IPOPT_NOP));

	//loss source and record 
	if(c == IPOPT_LSRR) {
		printf("received loss source and record routes!");
	} else if( c == IPOPT_SSRR) {
		printf("received strict source and record routes!");
	} else {
		printf("received option type %d\n",c);
		return;
	}
	
	printf("%s",inet_ntop(AF_INET,&hop1,str,sizeof(str)));

	len = *ptr++ - sizeof(struct in_addr);

	while(len > 0){
		printf("%s",inet_ntop(AF_INET,ptr,str,sizeof(str)));
		ptr += sizeof(struct in_addr);
		len -= sizeof(struct in_addr);
	}
	printf("\n");
	return 0;
}
