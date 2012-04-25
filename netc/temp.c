#include "unp.h"
#include <sys/utsname.h>

char **myhostname(int *addrtype)
{
	struct utsname name;
	struct hostent *ht;

	if(uname(&name) == -1)
		return (NULL);
	if((ht = gethostbyname(uname.nodename)) == NULL)
		return (NULL);
	*addrtype = ht->h_addrtype;
	return ht->h_addr_list;
}
