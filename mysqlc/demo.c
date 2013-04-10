#include <stdio.h>
#include <string.h>
#include "mysql.h"

int main(int argc,char *argv[])
{
	MYSQL *mysql;
	MYSQL_RES *results = NULL;
	MYSQL_ROW row;
	char	sql[1024];

	//初始化数据库
	mysql = mysql_init(NULL);

	//开始数据库连接
	if(!(mysql_real_connect(mysql,"localhost","casshern","goodbaby","test",0,NULL,0))){
		printf("mysql connect error!");
		exit(2);
	}

	//发送查询数据的请求
	if((mysql_query(mysql,"select stud_id,stud_name,age,birthday from student")) != 0){
		printf("mysql query error!");
		exit(3);
	}

	//获取查询数据集
	results = mysql_store_result(mysql);

	//循环获取每条数据
	while( ( row=mysql_fetch_row(results)) )
	{
		printf("%s - %s - %s - %s\n",row[0],row[1],row[2],row[3]);	
	}
	//释放结果集空间
	mysql_free_result(results);

	//关闭数据库连接
	mysql_close(mysql);

	//释放数据库内容
	mysql_library_end();
	return 0;
}
