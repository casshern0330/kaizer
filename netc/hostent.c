#include "unp.h"

int main(int argc,char *argv[])
{
	char *ptr,**pptr;
	char str[INET6_ADDRESTRLEN];
	struct hostent *hptr;

	while(--argc >0)
	{
		ptr = *++pptr;
		if((hptr = gethostbyname(ptr)) == NULL){
			err_msg("gethostbyname error for host: %s, %s",ptr,hstrerror(h_errno));
			continue;
		}

		printf("offical hostname :%s\n",hptr->h_name);

		for(pptr = hptr->h_aliases; *pptr != NULL; pptr++)
			printf("aliases:%s\n",*pptr);

		switch(hptr->h_addrtype){
			case AF_INET:
#ifdef AF_INET6
			case AF_INET6:
#endif
				pptr = hptr->h_addr_list;
				for(; *pptr != NULL; pptr++)
				{
					printf("\taddress:%s\n",
							inet_ntop(hptr->h_addrtype,*pptr,str,sizeof(str)));
					break;
				}

			default:
				err_type("unknown address type");
				break;
		}
	}
}
