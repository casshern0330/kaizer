#include "unp.h"

int main(int argc,char *argv[]){
	char **pname,**paddr;
	char addr[MAX_INET6];
	struct hostent *ht;
	int i=0;
	
	for(i=0;i<argc;i++){
		if((ht = gethostbyname(argv[i])) != NULL)
			err_sys("error gethostbyname %s",argv[i]);
			
		printf("offical name is %s\n",ht->h_cname);
		
		pname = ht->h_aliases;
		while(*pname != NULL){
			printf("alias name is %s\n",*pname++);
		}
		
		paddr = ht->h_addr_list;
		
		while(*paddr != NULL){
			printf("alias address name is %s\n",inet_ntop(ht->h_addrtype,*paddr,addr,MAX_INET6));
		}
	}
	return 0;
}

/*
typeof(paddr) == in_addr;
typeof(paddr) == in6_addr;
char* inet_ntop(int family,const void *paddr,char *buff,ssize_t size);

void* inet_pton(int family,const char *buff,void *paddr);
*/

