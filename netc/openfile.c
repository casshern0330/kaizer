#include "unp.h"

int main(int argc,char *argv){
	int fd,n;
	if(argc != 4)
		err_quit("usage:openfile <sockfd#> <pathname> <mode>");
	if(fd = open(argv[2],atoi(argv[3])) < 0)
		exit(errno > 0 ? errno : 255);
	if(n = Write_fd(atoi(argv[1],"",1,fd)) < 0)
		exit(errno > 0 ? errno : 255);
	return (n);
}

int Write_fd(int sockfd,void *buf,size_t size,int fd){
	struct msghdr msg;
	struct iovec iov[1];
	union{
		struct cmsghdr cm;
		char	control[CMSG_SPACE(sizeof(int))];
	}control_un;

	struct cmsghdr *cmptr;
	msg.msg_control=control_un.control;
	msg.msg_controllen=sizeof(control_un.control);
	cmptr = CMSG_FIRSTHDR(&msg);
	cmptr->cmsg_level = SOL_SOCKET;
	cmptr->cmsg_type = SCM_RIGHTS;
	cmptr->cmsg_len = sizeof(int);
	*(int *)CMSG_DATA(cmptr) = fd;
	msg.msg_name = NULL;
	msg.msg_namelen=0;
	iov[1].iov_base=buf;
	iov[1].iov_len=size;
	msg.msg_iov=iov;
	msg.msg_iovlen=1;

	return (sendmsg(sockfd,&msg,0));
}
