#include <unp.h>

int main(int argc,char *argv[])
{
	int cli;
	int rlen,wlen,n;
	struct sockaddr_in servaddr;
	struct addrinfo hints,*ai;
	char hostname[64],servicename[16],request[128],line[MAXLINE];

	if(argc < 2){
		perror("Usage : getWeb IP PORT serverPage");
		exit(1);
	}

	memset(request,0x00,sizeof(request));
	if(argc == 3){
			snprintf(request,sizeof(request),"GET / HTTP/1.1\r\n");
	}else if(argc == 4){
			snprintf(request,sizeof(request),"GET %s HTTP/1.1\r\n",argv[3]);
	}

	snprintf(request+strlen(request),sizeof(request)-strlen(request),"Host: %s\r\n",argv[1]);
	//strcat(request,"User-Agent: mozilla/5.0 (windows NT 6.1; WOW64; rv:15.0) Gecko/20100101 \r\n");
	strcat(request,"Accept: text/html\r\n");
	strcat(request,"Accept-Language: zh-CN\r\n");
	//strcat(request,"Connection: Keep-Alive \r\n");
	strcat(request,"\r\n");
	bzero(&hints,sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_flags = AI_CANONNAME;
	hints.ai_socktype = SOCK_STREAM;

	strcpy(hostname,argv[1]);
	strcpy(servicename,argv[2]);
	if((n=getaddrinfo(hostname,servicename,&hints,&ai)) != 0){
		perror("getaddrinfo error!");
		printf("error is :%s\n",gai_strerror(n));
		exit(1);
	}

	memcpy(&servaddr,ai->ai_addr,sizeof(struct sockaddr_in));
	freeaddrinfo(ai);
	if((cli = socket(AF_INET,SOCK_STREAM,0)) < 0){
		perror("socket error!");
		exit(1);
	}

	if(connect(cli,(struct sockaddr*)(&servaddr),sizeof(struct sockaddr_in)) != 0){
		perror("socket connect error!");
		exit(1);
	}

	wlen = write(cli,request,strlen(request));
	if(wlen <= 0) {
		close(cli);
		//printf("write to server error!error meg is %s\n",strerrno(errno));
		printf("write to server error!");
		exit(1);	
	}
	printf("%s",request);
	printf("has write %d to server\n",wlen);
	while((rlen = read(cli,line,MAXLINE)) > 0){
		line[rlen]=0;
		write(STDIN_FILENO,line,rlen);
	}
	if(rlen < 0)
			//printf("error msg is %s\n",strerrno(errno));
			printf("error read!\n");
	printf("last read is %d\n",rlen);
	close(cli);
	return 0;
}
