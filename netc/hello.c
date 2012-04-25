#include<stdio.h>

void inputS(FILE *fp,char *buf,size_t n);

int main(int argc,char *argv[])
{
	char line[200];
	printf("hello world!\n");
	inputS(stdin,line,200);
	printf("stdin fd = %d,stdout fd = %d,stderr fd = %d\n",fileno(stdin),fileno(stdout),fileno(stderr));
	exit(0);
}

void inputS(FILE *fp,char *buf,size_t n)
{
	fgets(buf,n,fp);
	printf("I just input is %s\n",buf);
}
