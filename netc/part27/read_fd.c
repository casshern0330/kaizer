#include "unp.h"

void write_fd(int fd,void *ptr,size_t nbytes,int sendfd)
{
	struct msghdr msg;
	struct iovec iov[1];
	bzero(&msg,sizeof(struct msghdr));
#ifdef  HAVE_MSGHDR_MSG_CONTROL
	union{
		struct cmsghdr cm;
		char	control[CMSG_SPACE(sizeof(int))];
	}control_un;

	struct cmsghdr *cmsg;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);

	cmsg = CMSG_FIRSTHDR(&msg);
	cmsg->cmsg_len = CMSG_LEN(sizeof(int));
	cmsg->cmsg_level = SO_SOCKET;
	cmsg->cmsg_type = 0;
	*((int *)CMSG_DATA(cmsg)) = sendfd;
#else
	msg.msg_accrights = (caddr_t)&sendfd;
	msg.msg_accrightslen = sizeof(int);
#endif

	msg.msg_name = NULL;
	msg.msg_namelen = 0;
	
	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	return (sendmsg(fd,&msg,0));
}


size_t read_fd(int fd,void *ptr,size_t nbytes,int *recvfd)
{
	struct msghdr msg;
	struct iovec iov[1];
	size_t	n;

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	union{
		struct cmsghdr cm;
		char	control[CMSG_SPACE(sizeof(int))];
	}control_un;
	struct cmsghdr *cmsg;

	msg.msg_control = control_un.control;
	msg.msg_controllen = sizeof(control_un.control);
#else
	int	newfd;
	msg.msg_accrights = (caddr_t*)&newfd;
	msg.msg_accrightslen = sizeof(int);

#endif
	msg.msg_name = NULL;
	msg.msg_namelen = 0;

	iov[0].iov_base = ptr;
	iov[0].iov_len = nbytes;
	msg.msg_iov = iov;
	msg.msg_iovlen = 1;

	if((n = recvmsg(fd,&msg,0)) <= 0)
		return (n);

#ifdef	HAVE_MSGHDR_MSG_CONTROL
	if((cmsg = CMSG_FIRSTHDR(&msg)) != NULL &&
			cmsg->cmsg_len == sizeof(int)) {
		if(cmsg->cmsg_level != SOL_SOCKET)
			err_quit("control level != SOL_SOCKET");
		if(cmsg->cmsg_type != SCM_RIGHTS)
			err_quit("control type != SCM_RIGHTS");
		*recvfd = *((int *)CMSG_DATA(cmsg));
	} else 
		*recvfd = -1;
	
#else
	if(msg.msg_accrightslen == sizeof(int)) 
		*recvfd = newfd;
	else 
		recvfd = -1;
	
#endif

	return (n);
}
