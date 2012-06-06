#include<stdlib.h>
#include<string.h>
#include<stdio.h>

#define MAX_BUF	200

long filesize(FILE *fd);

int inline Fread(char *buffer,size_t size,size_t len,FILE *stream)
{
	int num;
	num = fread(buffer,sizeof(char),len,stream);
	if(num < 0)
	{
		fprintf(stderr,"fread error");
		exit(EXIT_FAILURE);
	}
	return num;
}

int inline Fwrite(const void *buffer,size_t size,size_t len,FILE *stream)
{
	int num;
	num = fwrite(buffer,size,len,stream);
	if(num < 0)
	{
		fprintf(stderr,"fwrite error");
		exit(EXIT_FAILURE);
	}
	return num;
}

int main(int argc,char *argv[])
{
	char buffer[MAX_BUF];
	FILE *from_fd,*to_fd;
	int read_num,write_num;
	long file_len;

	if(argc != 3){
		fprintf(stderr,"usage:copy <from_file> <to_file>");
		exit(EXIT_FAILURE);
	}

	if((from_fd = fopen(argv[1],"rb")) == NULL){
		fprintf(stderr,"open %s error\n",argv[1]);
		exit(EXIT_FAILURE);
	}

	if((to_fd = fopen(argv[2],"wb")) == NULL){
		fprintf(stderr,"open %s error\n",argv[22]);
		exit(EXIT_FAILURE);
	}

	file_len = filesize(from_fd);
	printf("%s file size is %ld\n",argv[1],file_len);

	while(!feof(from_fd)){
		read_num = Fread(buffer,sizeof(char),MAX_BUF,from_fd);
		if(MAX_BUF > file_len)
		{
			write_num = Fwrite(buffer,sizeof(char),file_len,to_fd);
			break;
		}else
		{
			write_num = Fwrite(buffer,sizeof(char),MAX_BUF,to_fd);
		}
		file_len -= write_num;
		bzero(buffer,MAX_BUF);
	}
	fclose(from_fd);
	fclose(to_fd);
	exit(1);
}

/*
 * @description:获取文件当前位置保存到变量中,设置当前位置到文件结尾，获取文件长度，从新设置文件位置到当前位置
 * @return:返回文件的长度
 * @parameter:stream,文件流
 */
long filesize(FILE *stream)
{
	long len,curpos;
	curpos = ftell(stream);	
	fseek(stream,0L,SEEK_END);
	len = ftell(stream);
	fseek(stream,curpos,SEEK_SET);
	return len;
}
