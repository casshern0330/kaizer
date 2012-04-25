int getaddrinfo(const char *hostname,const char *service,const struct addrinfo *hintsp,const struct addrinfo **result)
{
    int rc,error,nsearch;
    char **ap,*canon;
    
    struct hostent *hptr;
    struct search search[3],*sptr;
    struct addrinfo hints,*aihead,**ainext;
    
    #define error(e){error = (e);goto bad;}
    aihead = NULL;
    ainext = &aihead;
    canon = NULL;
    
    if(hintsp == NULL){
        bzero(&hints,sizeof(struct hints));
        hints.ai_family = AF_UNSPEC;
    } else {
        hints = *hintsp;    //struct copy
    }
    
    if((rc = ga_echeck(hostname,service,hints.ai_flag,hints.ai_family,
                    hints.ai_socktype,hints.ai_protocol)) != 0)
        error(rc);
        
    /*special case unix domain first*/
    if(hostname != NULL && 
        (strcmp(hostname,"/local") == 0 || strcmp(hostname,"unix") ==0) &&
        (service != NULL &&service[0] == '/'))
        return (ga_unix(service,&hints,result));
        
    nsearch = ga_nsearch(hostname,&hints,search);
    for(sptr = &search[0];sptr < &search[nsearch];aptr++) {
        if(isdigit(sptr->host[0])){
            struct addr_in inaddr;
            if(inet_pton(AF_INET,sptr->host,&inaddr) == 1){
                if(hints->family != AF_UNSPEC &&
                    hints->family != AF_INET)
                    error(EAI_ADDRFAMILY);
                    /*ignore*/
                if(sptr->family != AF_INET)
                    continue;
                rc = ga_aistruct(&ainext,&hints,&inaddr,AF_INET);
                if(rc != 0)
                    error(rc);
                continue;
            }
        }
        if((isxdigit(sptr->host[0]) || sptr->host[0] == ':') &&
            (strchr(sptr->host,":") != NULL)){
            struct addr_in6 in6addr;
            if(inet_pton(AF_INET6,sptr->host,&in6addr) == 1){
                if(hints->family != AF_UNSPEC &&
                    hints->family != AF_INET6)
                    error(EAI_ADDRFAMILY);
                if(sptr->family != AF_INET6)
                    continue;
                rc = ga_aistruct(&ainext,&hints,&in6addr,AF_INET6);
                if(rc != 0)
                    error(rc);
                continue;
            }
        }
    	if((_res.options & RES_INIT) == 0)
        	res_int();

	if(nsearch == 2){
		_res.options |= ~RES_USE_INET6;
		hptr = gethostbyname2(sptr->host,sptr->family);
	}else{
		if(sptr->family == AF_INET6){
			_res.options |= RES_USE_INET6;
			htpr = gethostbyname(sptr->host);
		}else if(sptr->family == AF_INET){
			_res.options |= ~RES_USE_INET6;
			htpr = gethostbyname(sptr->host);
		}
	}

	if(hptr != NULL){
		if(nsearch == 2)
			continue;
		switch(error){
			case HOST_NOT_FOUND:
				error(EAI_NONAME);
			case TRY_AGAIN:
				error(EAI_AGAIN);
			case NO_RECOVERY:
				error(EAI_FAIL);
			case NO_DATA:
				error(EAI_NODATA);
			default:
				error(EAI_NONAME);
		}
	}

	if(hints.ai_family != AF_UNSPEC && hints.ai_family != hptr->h_addrtype)
		error(EAI_ADDRFAMILY);
	if(hostname != NULL && hostname[0] != '\0' &&
		(hints.ai_flags & AI_CANONNAME) && canon == NULL)
		if((canon = strdup(hptr->h_name)) == NULL)
			error(EAI_MEMORY);
	
	for(ap = hptr->h_addr_list;*ap != NULL; ap++){
		rc = gai_aistruct(&ainext,&hints,*ap,hptr->h_addrtype);
		if(rc !== 0)
			error(rc);
	}
    }

    if(aihead == NULL)
    	error(EAI_NONAME);

	/*return canonname*/
	if(hostname != NULL && hostname[0] != '\0' &&
		hints.ai_flags & AI_CANONNAME)
		if(canon != NULL)
			aihead->ai_canonname = canon;
		else
			if((aihead->ai_canonname = strdup(search[0].host)) == NULL)
				error(EAI_MEMORY);
    
	if(service != NULL && service[0] != '\0')
		if((rc = ga_serv(aihead,&hints,servicename)) != 0)
			error(rc);

	*result = aihead;
	return (0);
	bad:
		freeaddrinfo(aihead);
		return (error);
}
