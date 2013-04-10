#include "unp.h"
int my_open(char *pathname,int mode);
int Read(int sockfd,void *buf,size_t size,int *recvfd);

int main(int argc,char *argv[]){
	int n;
	char buffer[MAXLINE];
	if(argc != 2)
		err_quit("usage mycat <filepath>");

	if((fd = my_open(argv[1],O_READONLY)) < 0)
		err_sys("error get file descriptor!");
	while((n=Read(fd,buffer,MAXLINE)) <0)
		Write(fd,buffer,n);
	exit(0);
}


int my_open(char *pathname,int mode){
	int		sockfd[2],fd,state;
	pid_t	childpid;
	char	argmode[10];
	char	argsock[10];
	socketpair(AF_LOCAL,SOCK_STREAM,0,sockfd);
	if((childpid=fork()) == 0){
		close(sockfd[0]);
		snprintf(argmode,sizeof(argmode),"%d",mode);
		snprintf(argsock,sizeof(argsock),"%d",sockfd[1]);
		execl("./openfile","openfile",argsock,pathname,argmode,(char *)NULL);
		err_sys("execl error");
	}
	close(sockfd[0]);

	waitpid(childpid,&state,0);

	if(WIFEXITED(status) == 0)
		err_quit("child not terminated");
	if((status = WEXITSTATUS(status)) == 0)
		Read_fd(sockfd[1],&c,&fd);
	else{
		errno = status;
		fd=-1;
	}
    close(fd[1]);
	return (fd);
}

int Read_fd(int sockfd,void *ptr,size_t nbyte,int *fd){
	struct msghdr msg;
	struct iovec iov[1];
	size_t n;
	struct cmsghdr *cmsg;
	union{
		struct cmsghdr control;
		char control[CMSG_SPACE(sizeof(int))];
	}control_un;
	msg.msg_control=control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
	msg.msg_name=NULL;
	msg.msg_namelen=0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbyte;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;
	if((n = recvmsg(sockfd,&msg,0)) <= 0)
		return (n);
	if((cmsg = CMSG_FIRSTHDR(&msg)) != NULL && cmsg->cmsg_len == CMSG_LEN(sizeof(int))){
		if(cmsg->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if(cmsg->cmsg_type != SCM_RIGHTS)
			err_quit("control type != SCM_RIGHTS");
		*fd = *(int *)CMSG_DATA(cmsg);
	}else{
		*fd = -1;
	}
	return (n);
}
