#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/times.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdarg.h>
#include <syslog.h>
#include <signal.h>
#include <unp.h>

#define  DEBUG 1

#ifndef MYBUF_SIZE
    #define MYBUF_SIZE 102400
#endif

#ifndef CONN_TIMEOUT
    #define CONN_TIMEOUT 20000000
#endif


struct sock_mgr
{
  int                cli_id;
  int                srv_id;
  struct sock_mgr    *next;
};

struct addr_mgr
{
  char               ip[32];
  int                port;
  struct sockaddr_in sock_addr;
  struct addr_mgr    *next;
};

void sig_init()
{
    struct sigaction sa;
    sa.sa_handler = SIG_IGN;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGPIPE, &sa, 0);
}


char * now()
{
    time_t now_t;
    now_t=time(NULL);
    static char buf[64];
    memset(buf,0,sizeof(buf));
    struct tm *timeptr = localtime(&now_t);
    sprintf(buf,"%04d%02d%02d%02d%02d%02d",
                timeptr->tm_year + 1900,timeptr->tm_mon + 1,timeptr->tm_mday,
                timeptr->tm_hour,timeptr->tm_min,timeptr->tm_sec);
    return buf;
    
}
void wf(char *file_name,char *buf, size_t len)
{
    int fd;
    char tmp[512];
    fd = open( file_name,
        O_WRONLY | O_CREAT | O_APPEND ,
        S_IRUSR | S_IWUSR );
    char *ptime = now();
    sprintf(tmp,">>>>>%s:pkt_size(%ld)[",ptime,len);
    write(fd,tmp,strlen(tmp));
    write(fd,buf,len);
    sprintf(tmp,"]<<<<<\r\n");
    write(fd,tmp,strlen(tmp));
    close(fd);
}

int check_fd_is_valid(int fd)
{
    struct stat st;
    return fstat(fd,&st);
}



int myconnect(int fd, const struct sockaddr * serv_addr, socklen_t serv_addr_len, int timeout)
{
    int old_flags = 0;
    int new_flags = 0;
    fd_set set;
    struct timeval tm = {timeout/1000000, timeout%1000000};   /* microseconds */
    int error = 0;
    int len = sizeof(int);
    int retcode = 0;
    if ( (old_flags=fcntl(fd, F_GETFL)) == -1) 
    {
        printf("%s:%d fcntl F_GETFL failed:[%d:%s]\n", __FILE__,__LINE__,errno,strerror(errno));
        close(fd);
        return -1;
    }
    new_flags = old_flags;
    new_flags |= O_NONBLOCK;
    if (fcntl(fd, F_SETFL, new_flags) == -1) 
    {
        printf("%s:%d fcntl F_SETFL failed:[%d:%s]\n", __FILE__,__LINE__,errno,strerror(errno));
        close(fd);
        return -2;
    }
    if( connect(fd, serv_addr, serv_addr_len) == -1 )
    {
        FD_ZERO(&set);
        FD_SET(fd, &set);
        if( select(fd+1, NULL, &set, NULL, &tm) > 0)
        {
            getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, (socklen_t *)&len);
            if(error == 0)
            {
                retcode = 0;
            }
            else
            {
                retcode = -3;
            }
        }
        else 
        {
            retcode = -4; /* conn timeout */
        }
    }
    else
    {
        retcode = 0;
    }
    
    fcntl(fd, F_SETFL, old_flags);
    if(0 != retcode)
    {
        close(fd);
    }
    return retcode;
}


int main(int argc,char **argv)
{
  errno = 0;
  printf("$$$$$$$$$$$$$$$$$FD_SETSIZE=%d\n",FD_SETSIZE);
  int sys_max_fdsize = FD_SETSIZE - 2;
  int conn_timeout = CONN_TIMEOUT;
  int retcode;
  int i,err_num;
  int rw_flag=0;
  socklen_t clilen;
  struct sock_mgr sockhead,*sockptr,*sptr;
  struct addr_mgr addrhead,*addrptr,*aptr;
  int listenid,connid,optinfo,optlen,srvport;
  struct sockaddr_in srv_addr,cli_addr;
  struct timeval tval;
  fd_set rset;
  int fdret,maxid,rlen,wlen;
  char buf[MYBUF_SIZE+1];
  sig_init();
  if(argc<3 || argc%2==1 || (srvport=atoi(argv[1]))<14 || srvport>65535)
  {
    printf("Usage:%s srvport IP port [IP port] ...\n",argv[0]);
    return(1);
  }

  memset(&addrhead,0x0,sizeof(addrhead));
  aptr=&addrhead;

  for(i=2;i<argc;i+=2)
  {
    addrptr=(struct addr_mgr *)malloc(sizeof(struct addr_mgr));
    memset(addrptr,0x0,sizeof(struct addr_mgr));

    strncpy(addrptr->ip,argv[i],30);
    addrptr->port=atoi(argv[i+1]);

    addrptr->sock_addr.sin_family=AF_INET;
    addrptr->sock_addr.sin_addr.s_addr=inet_addr(addrptr->ip);
    addrptr->sock_addr.sin_port=htons(addrptr->port);

    if(addrptr->sock_addr.sin_addr.s_addr==-1 || addrptr->port<24 || addrptr->port>65536)
    {
      printf("Incorrect IP or Port[%s].[%s]\n",argv[i],argv[i+1]);
      break;
    }

    addrptr->next=NULL;
    aptr->next=addrptr;
    aptr=aptr->next;
  }

  if(i<argc)
  {
    printf("Usage:%s srvport IP port [IP port] ...\n",argv[0]);

    while(addrhead.next)
    {
      addrptr=addrhead.next;
      addrhead.next=addrptr->next;

      free(addrptr);
    }
      
    exit(1);
  }

  aptr->next=addrhead.next;  /* liupengc Iz3IR;8v4xSPM7=Z5c5D Q-;7A41m */
  aptr=addrhead.next;

  /*** TEST ******/
  for(i=0,addrptr=addrhead.next;i<10;i++,addrptr=addrptr->next)
    printf("[%d] %s:%d\n",i,addrptr->ip,addrptr->port);
  

  listenid=socket(AF_INET,SOCK_STREAM,0);
  if(listenid<0)
  {
    printf("socket() failed[%d]\n",errno);
    return(1);
  }

  memset(&srv_addr,0x0,sizeof(srv_addr));
  srv_addr.sin_family=AF_INET;
  srv_addr.sin_addr.s_addr=0;
  srv_addr.sin_port=htons(srvport);

  optinfo=1;
  optlen=sizeof(optinfo);
  setsockopt(listenid,SOL_SOCKET,SO_REUSEADDR,(void *)&optinfo,optlen);

  if(bind(listenid,(struct sockaddr *)&srv_addr,sizeof(srv_addr))<0)
  {
    printf("bind(%d) failed[%d]\n",listenid,errno);
    return(1);
  }

  if(listen(listenid,25)<0)
  {
    printf("listen(%d) failed[%d]\n",listenid,errno);
    return(1);
  }

  memset(&sockhead,0x0,sizeof(sockhead));

  for(;;)
  {
    FD_ZERO(&rset);
    FD_SET(listenid,&rset);
    if(check_fd_is_valid(listenid))
    {
        printf("*********** listenid=%d is invalid fd\n", listenid);
    }
    maxid=listenid;

    for(sockptr=sockhead.next;sockptr;sockptr=sockptr->next)
    {
      
      if(check_fd_is_valid(sockptr->cli_id))
      {
        printf("*********** sockptr->cli_id=%d is invalid fd\n", sockptr->cli_id);
      }
      
      FD_SET(sockptr->cli_id,&rset);
      if(maxid<sockptr->cli_id)
        maxid=sockptr->cli_id;

      if(check_fd_is_valid(sockptr->srv_id))
      {
        printf("*********** sockptr->srv_id=%d is invalid fd\n", sockptr->srv_id);
      }
      
      FD_SET(sockptr->srv_id,&rset);
      if(maxid<sockptr->srv_id)
        maxid=sockptr->srv_id;
    }

    maxid++;

    fdret=select(maxid,&rset,NULL,NULL,NULL);
    if(fdret<0)
    {
      printf("select(maxid=%d) failed[%d:%s]\n",maxid,errno,strerror(errno));
      exit(1);
    }

    if(FD_ISSET(listenid,&rset))
    {
      memset(&cli_addr,0x0,sizeof(cli_addr));
      clilen=sizeof(cli_addr);

      connid=accept(listenid,(struct sockaddr *)&cli_addr,&clilen);
      if(connid<0)
      {
        printf("accept(listenid=%d) failed[%d:%s]\n",listenid,errno,strerror(errno));

        if(++err_num>1000)
        {
          printf("accept over the mark 1000 times failed!\n");
          exit(1);
        }
        else
          continue;
      }
      err_num=0;
      printf("connect from %s:%d [connid=%d]\n",(char *)inet_ntoa(cli_addr.sin_addr),(cli_addr.sin_port),connid);
      if(connid > sys_max_fdsize)
      {
        close(connid);
        printf("!!!!!! !!!!!! !!!!!!! the numbers[%d] of the client is more than system maxsize[%d]\n",connid, sys_max_fdsize);
        usleep(500000);
        continue;
      }
      sptr=(struct sock_mgr *)malloc(sizeof(struct sock_mgr));
      memset(sptr,0x0,sizeof(sptr));

      sptr->cli_id=connid;
      for(i=0;;i++)
      {
        sptr->srv_id=socket(AF_INET,SOCK_STREAM,0);
        if(sptr->srv_id<0)
        {
          printf("socket() failed[%d:%s]\n",errno,strerror(errno));
          exit(1);
        }

        /* if(connect(sptr->srv_id,(struct sockaddr *)&aptr->sock_addr,sizeof(aptr->sock_addr))<0) */
        if( (retcode=myconnect(sptr->srv_id,(struct sockaddr *)&aptr->sock_addr,sizeof(aptr->sock_addr), conn_timeout))<0)
        {
          printf("sockagent connect(srv_id=%d) failed(%d)[%d:%s][%s:%d]!\n",sptr->srv_id,retcode,errno,strerror(errno),aptr->ip,aptr->port);
          close(sptr->srv_id);
          aptr=aptr->next;  /* aptr->next 3"JTA,=SOBR;8v5XV7 */
          
          /*if(i>5)  3"JTA,=S;7VPKySP5D5XV7J'0\:s,9X1U?M;'6KA,=S */
          if(aptr == aptr->next)
          {
            printf("connect failed ,close!\n");
            close(sptr->cli_id);
            /* close(sptr->srv_id); RQ>-9X1UR;4N,2;PhR*VXPB9X1U */
            free(sptr);
            break;
          }
        }
        else
        {
          printf("sockagent connect %s:%d OK\n",aptr->ip,aptr->port);
          /* N*:NUbQyIhVC2;:CJ9
          struct timeval tmptm;
          tmptm.tv_sec = 3;
          tmptm.tv_usec = 0;
          setsockopt(sptr->srv_id, SOL_SOCKET, SO_RCVTIMEO, &tmptm, sizeof(tmptm));
          */
          
          aptr=aptr->next; /* BVQ/J=A,=S */

          sptr->next=sockhead.next;
          sockhead.next=sptr;

          break;
        }
      }

      continue; /* JU5=?M;'6KGkGs:s, A,=SR;8v7~Nq6K, <LPxselect */
    }

    for(sockptr=&sockhead;sockptr->next;sockptr=sockptr->next)
    {
      if(FD_ISSET(sockptr->next->cli_id,&rset))
      {
        rlen=read(sockptr->next->cli_id,buf,MYBUF_SIZE);
        if(rlen<=0)
        {
          printf("read from cli_id(%d) rlen=(%d) [%d:%s]\n",sockptr->next->cli_id,rlen,rlen!=0?errno:0,rlen!=0?strerror(errno):"client close conn");
          close(sockptr->next->cli_id);
          close(sockptr->next->srv_id);
          sptr=sockptr->next;
          sockptr->next=sptr->next;
          free(sptr);
          break;
        }
#ifdef DEBUG       
        wf("cli.data",buf, rlen); /* added by liupengc */
        printf("请求长度:%d\nrequest info:%s\n",rlen,buf);
#endif
        wlen=write(sockptr->next->srv_id,buf,rlen);
        if(wlen<0)
        {
          printf("write to srv(%d) wlen=(%d) failed[%d:%s]\n",sockptr->next->srv_id,wlen,errno,strerror(errno));
          close(sockptr->next->cli_id);
          close(sockptr->next->srv_id);
          sptr=sockptr->next;
          sockptr->next=sptr->next;
          free(sptr);
          break;
        }
      }

      if(FD_ISSET(sockptr->next->srv_id,&rset))
      {
        rlen=read(sockptr->next->srv_id,buf,MYBUF_SIZE);
        buf[rlen]='\0';
        if(rlen<=0)
        {
          printf("read from srv_id(%d) rlen=(%d) [%d:%s]\n",sockptr->next->srv_id,rlen,rlen!=0?errno:0,rlen!=0?strerror(errno):"server close conn");
          close(sockptr->next->cli_id);
          close(sockptr->next->srv_id);
          sptr=sockptr->next;
          sockptr->next=sptr->next;
          free(sptr);
          break;
        }

        wlen=write(sockptr->next->cli_id,buf,rlen);
#ifdef DEBUG
        wf("srv.data",buf, rlen); /* added by liupengc */
        printf("相应长度:%d\nresponse info:%s\n",rlen,buf);
#endif
        if(wlen<0)
        {
          printf("write to cli(%d) wlen=(%d) failed[%d:%s]\n",sockptr->next->cli_id,wlen,errno,strerror(errno));
          close(sockptr->next->cli_id);
          close(sockptr->next->srv_id);
          sptr=sockptr->next;
          sockptr->next=sptr->next;
          free(sptr);
          break;
        }
      }
    }
  }
}
