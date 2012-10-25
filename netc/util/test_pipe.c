
int main(int argc,char *argv)
{
	int	n,fd[2];
	char	line[MAXLINE];
	pid_t	pid;

	if(pipe(fd) < 0£©
		err_sys("pipe error");

	if((pid = fork()) == 0){
		Close(fd[1]);
		n = read(fd[0],line,MAXLINE);
		write(STDOUT_FILENO,line,n);
	}else{
		Close(fd[0]);
		write(fd[1],"hello world\n",12);
	}
	exit(0);
}
