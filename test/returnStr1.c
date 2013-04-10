#include <stdio.h>

char *returnStr();

int main(int argc,char *argv[])
{
	char *str;
	str = returnStr();
	printf("str = %s\n",str);
	return 0;
}

char *returnStr() {
	char p[] = "hello world!";
	return p;
}
