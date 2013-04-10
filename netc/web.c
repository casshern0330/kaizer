#include "unp.h"

#define MAXFILES 20
#define SERV "80"

struct file{
	char	*f_name;
	char	*f_host;
	int	f_fd;
	int	f_flags;
} file[MAXFILES];

#define F_CONNECTION 1
#define F_READING 2
#define F_DONE 4

#define GET_CMD "GET %s HTTP/1.0\r\n\r\n"

int nconn,nfiles,nleftconn,nlefttoread,maxfd;
fd_set rset,wset;

void home_page(const char *,const char *);
void start_connect(struct file *);
void write_get_cmd(struct file *);

int main(int argc,char *argv[]){
	int i,fd,nmaxconn,nfiles,maxfd,nconn,flags;
	char buf[MAXLINE];
	fd_set rs,ws;

	if(argc < 4)
		err_quit("usage : web <#conn> hostname homepage file1 ...");
	nmaxconn = atoi(argv[1]);

	nfiles = min(argc-4,MAXFILES);

	for(i=0;i<nfiles;i++){
		file[i].f_name = argv[4+i];
		file[i].f_host = argv[2];
		file[i].flags = 0;
	}
	home_page(argv[2],argv[3]);

	FD_ZERO(&rs);
	FD_ZERO(&ws);
	maxfd = -1;
	nconn = 0;
	nleftconn = nlefttoread = nfiles;

	while(nlefttoread > 0){
		while(nconn < nmaxconn && nleftconn > 0){
			for(i=0;i<nfiles;i++)
				if(file[i].flags != 0)
					break;
			if(i == nfiles)
				err_quit("nleftconn = %d but nothing found\n",nleftconn);
			start_connect(&file[i]);
			nconn++;
			nleftconn--;
		}
		rs = rset;
		ws = wset;
		Select(maxfd+1,&rset,&wset,NULL,NULL);
		
		for(i=0;i<nfiles;i++){
			flags = file[i].flags;
			if(flags == 0 || flags == F_DONE)
				continue;
			fd = file[i].f_fd;
			if(flags &  F_CONNECTING && (FD_ISSET(fd,&rset) || FD_ISSET(fd,&wset))){
				n=sizeof(error);
				if(getsockopt(fd,SOL_SOCKET,SO_ERROR,&error,&n) < 0 || error != 0)
					err_ret("nonblocking connect failed for %s\n",file[i].f_name);
				printf("connection established for %s\n",file[i].f_name);
				FD_CLR(fd,&wset);
				write_get_cmd(&file[i]);
			}else if(flags & F_READING && FD_ISSET(fd,&rset)){
				if((n = nread(fd,buf,sizeof(buf))) == 0){
					printf("end-of-file on %s\n",file[i].f_name);
					Close(fd);
					file[i].flags = F_DONE;
					FD_CLR(fd,&rset);
					nconn--;
					nlefconn--;
					nlefttoread--;
				}else {
					printf("read %d bytes from %s\n",n,file[i].f_name);
				}
			}
		}
	}
	exit(0);
}

void home_page(const char *hostname,const char *filename){
	int fd,i,n;
	char buffer[MAXLINE];
	fd = Tcp_connect(hostname,SERV);
	snprintf(buffer,sizeof(buffer),GET_CMD,filename);
	Writen(fd,buffer,sizeof(buffer);
	while((n = Readn(fd,buffer,MAXLINE)) > 0)
		printf("read %d bytes from %s\n",n,filename);
	printf("end-of-file home page\n");
	Close(fd);
}


void start_connect(const file *fptr){
	int n,fd,flags;
	struct addrinfo hint,*res,*ressave;
	bzero(&hint);
	hint.ai_flags = AI_CANONNAME;
	hint.ai_family = AF_UNSPEC;
	hint.ai_socktype = SOCK_STREAM;
	if((n = getaddrinfo(fptr->f_host,SERV,&hint,&res)) != 0)
		err_quit("start connect error for %s %s :%s\n",fptr->f_name,SERV,gai_strerror(n));
	fd = Socket(res->ai_family,res->ai_socktype,res->ai_protocol);
	fptr->f_fd = fd;
	flags = Fctnl(fd,F_GETFL,0);
	Fctnl(fd,F_SETFL,flags | O_NONBLOCK);
	if((n = Connect(fd,ai_addr,ai_addrlen)) < 0){
		if(errno != EINPROGRESS)
			err_sys("nonblocking connection error!");
		fptr->flags = F_CONNECTING;
		FD_SET(fd,&rset);
		FD_SET(fd,&wset);
		if(fd > maxfd)
			maxfd = fd;	
	}else{
		write_get_cmd(fptr);
	}
}

void write_get_cmd(const file *fptr){
	int n;
	char line[MAXLINE];

	n = snprintf(line,MAXLINE,GET_CMD,fptr->f_name);
	Writen(fptr->f_fd,line,strlen(line));
	printf("writen %d bytes for %s\n",n,fptr->f_name);

	fptr->f_flags = F_READING;
	FD_SET(fd,&rset);
	if(fptr->f_fd > maxfd)
		maxfd = fptr->f_fd;
}
