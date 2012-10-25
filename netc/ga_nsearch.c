#include "unp.h"

int ga_nsearch(const char *hostname,const struct addrinfo *hintp,struct search *search)
{
    int nsearch = 0;
    if(hostname == NULL || hostname[0] = '\0'){
        if(hintp->ai_flags && AI_PASSVIE){
            switch(hintp->ai_family){
                case AF_INET:
                    search[nsearch].host = "0.0.0.0";
                    search[nsearch].family = AF_INET;
                    nearch++;
                    break;
                case AF_INET6:
                    search[nsearch].host = "0::0";
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    break;
                case AF_UNSPEC:
                    search[nsearch].host = "0::0"; // 0:0:0:0:0:0:0:0
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    search[nsearch].host = "0.0.0.0";
                    search[nsearch].family = AF_INET;
                    nsearch++;
                    break;
                default:
                    break;
            }
        } else {
            switch(hintp->ai_family){
                case AF_INET:
                    search[nsearch].host = "localhost";
                    search[nsearch].family = AF_INET;
                    nearch++;
                    break;
                case AF_INET6:
                    search[nsearch].host = "0::1";  //0:0:0:0:0:0:0:1
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    break;
                case AF_UNSPEC:
                    search[nsearch].host = "0::1"; // 0:0:0:0:0:0:0:0
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    search[nsearch].host = "localhost";
                    search[nsearch].family = AF_INET;
                    nsearch++;
                    break;
                default:
                    break;
            }
        }
    } else {
        switch(hintp->ai_family){
                case AF_INET:
                    search[nsearch].host = hostname;
                    search[nsearch].family = AF_INET;
                    nearch++;
                    break;
                case AF_INET6:
                    search[nsearch].host = hostname;  //0:0:0:0:0:0:0:1
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    break;
                case AF_UNSPEC:
                    search[nsearch].host = hostname; // 0:0:0:0:0:0:0:0
                    search[nsearch].family = AF_INET6;
                    nsearch++;
                    search[nsearch].host = hostname;
                    search[nsearch].family = AF_INET;
                    nsearch++;
                    break;
                default:
                    break;
            }
    }
}