#include "unp.h"
#include <sys/utsname.h>

struct sockaddr **my_addr(int *addrtype) {
	struct hostent *ht = NULL;
	struct utsname myname;

	if(uname(&myname) == -1 )
		return (NULL);
	if((ht = gethostbyname(myname.nodename)) == NULL)
		return (NULL);
	*addrtype = ht->h_addrtype;
	return (SA**)ht->h_addr_list;
}
