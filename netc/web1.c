#define MAXFILES 20

struct myfile{
    char *f_name;
    char *f_host;
    int f_fd;
    int f_flags;
}myfile[MAXFILES];

#define CMD_GET "GET %s HTTP1.0 \r\n\r\n"
#define SERV    "80"

#define F_CONNECTING 1
#define F_READING 2
#define F_DOWN 4

void start_connection(struct myfile *f,int *maxfd,fd_set *rset,fd_set *wset);
void cmd_send_msg(struct myfile *f,int *maxfd,fd_set *rset,fd_set *wset);

int main(int argc,char *argv){
    int maxfd,fd,flags,nleftconn,nleftread,maxconn,maxfile,nconn,n,i;
    fd_set rset,wset,grset,gwset;
    
    if(argc < 4)
        err_quit("usage:web <#conn> <hostname> <homepage> file1 ...");
    
    FD_ZERO(&rset);
    FD_ZERO(&wset);
    FD_ZERO(&grset);
    FD_ZERO(&gwset);
    
    maxfd = -1;
    nconn = 0;
    maxconn = atoi(argv[1]);
    maxfile = min(argc-4,MAXFILES);
    nleftconn = nleftread = maxfile;
    for(i=0;i<maxfile;i++){
        myfile[i].f_name = argv[i+4];
        myfile[i].f_host = argv[2];
        myfile[i].f_flags = 0;
    }
    
    home_page();
    
    while(nleftread > 0){
        while(nleftconn >0 && nconn < maxconn){
            for(i=0;i<maxfile;i++)
                if(myfile[i].f_flags == 0)
                    break;
            if(i == maxfile)
                err_sys("left %d conn need,but found nothing",nleftconn);
            start_connection(myfile[i],&maxfd,&grset,&gwset);
            nconn--;
            nleftconn--;
        }
        
        for(i=0;i<maxfile;i++){
            flags = myfile[i].f_flags;
            if(flags == F_DOWN)
                continue;
            fd = myfile[i]
            
            rset = grset;
            wset = gwset;
            Select(maxfd,&rset,&wset,NULL,NULL);
            
            if(flags & F_CONNECTING && (FD_ISSET(fd,&rset) || FD_ISSET(fd,&wset))){
                n = sizeof(error);
                if(getsockopt(fd,SOL_SOCKET,SO_ERROR,&error,&n) < 0 || error != 0)
                    err_ret("nonblocking connect failed for %s\n",myfile[i].f_name);
                printf("connect established for %s\n",myfile[i].f_name);
                FD_CLR(fd,*gwset);
                write_get_cmd(&myfile[i],&maxfd,&grset);
            }
            
            if((flags & F_READING) && FD_ISSET(fd,&rset)){
                n = Readn(myfile[i].f_fd,buf,MAXLINE);
                if(n == 0){
                    printf("end-of-file on %s\n",myfile[i].f_name);
                    myfile[i].f_flags = F_DONE;
                    FD_CLR(fd,&grset);
                    Close(fd);
                    nleftread--;
                    nleftconn--;
                    nconn--;
                }else{
                    printf("read %d bytes from %s\n",n,myfile[i].f_name);
                }
            }
        }
    }
}

void start_connection(struct myfile *f,int *maxfd,fd_set *rset,fd_set *wset){
    int fd,flags,n,i;
    struct addrinfo hints,*res;
    bzero(&hints);
    hints.ai_flags = AI_CANONNAME;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if((n = getaddrinfo(f->f_host,SERV,&hints,&res) != 0)
        err_quit("getaddrinfo error for %s %s:%s",f->f_host,SERV,gai_strerror(n));
    
    fd = Socket(res->ai_family,res->ai_socktype,res->ai_protocol);
    flags = fctnl(fd,F_GETFL,0);
    fctnl(fd,F_SETFL,flags | O_NONBLOCK);
    
    if(connect(fd,res->ai_addr,res->ai_addrlen) < 0){
        if(errno != EINPROGRESS){
            Close(fd);
            err_quit("nonblock connection to %s error!",f->f_host);
        }
        f->f_fd = fd;
        f->f_flags = F_CONNECTING;
        FD_SET(fd,rset);
        FD_SET(fd,rset);
        
        if(fd > *maxfd)
            *maxfd = fd;
    }else{
        write_get_cmd(f,maxfd,rset);
    }
}

void write_get_cmd(struct myfile *f,int *maxfd,fd_set *rset){
    char buf[MAXSIZE];
    int i,n;
    snprintf(buf,MAXSIZE,CMD_GET,f->f_name);
    n = Writen(f->f_fd,buf,strlen(buf));
    printf("write %d bytes to %s\n",n,f->f_host);
    f->f_flags = F_READING;
    FD_SET(f->f_fd,rset);
    if(fd > *maxfd)
        *maxfd = fd;
}
