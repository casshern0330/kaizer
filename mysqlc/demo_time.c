#include <stdio.h>
#include <string.h>
#include "mysql.h"

int main(int argc,char *argv[])
{
	MYSQL *mysql;
	MYSQL_RES *results = NULL;
	MYSQL_ROW row;
	char	sql[1024];
	MYSQL_TIME	ts;
	MYSQL_BIND	bind[4];
	MYSQL_STMT	*stmt;

	//初始化数据库
	mysql = mysql_init(NULL);

	//开始数据库连接
	if(!(mysql_real_connect(mysql,"localhost","casshern","goodbaby","test",0,NULL,0))){
		printf("mysql connect error!");
		exit(2);
	}

	strcpy(sql,"insert into test_time(time_field,date_field,timestamp_field,datetime_field) values(?,?,?,?)");

	stmt = mysql_stmt_init(mysql);

	if(!stmt){
		perror("mysql_stmt_init error\n");
		exit(0);
	}

	if(mysql_stmt_prepare(stmt,sql,strlen(sql))) {
		perror("mysql_stmt_prepare error\n");
		exit(0);
	}

	bind[0].buffer_type = MYSQL_TYPE_TIME;
	bind[0].buffer = (char *)&ts;
	bind[0].is_null = 0;
	bind[0].length = 0;


	bind[1].buffer_type = MYSQL_TYPE_DATE;
	bind[1].buffer = (char *)&ts;
	bind[1].is_null = 0;
	bind[1].length = 0;

	bind[2].buffer_type = MYSQL_TYPE_TIMESTAMP;
	bind[2].buffer = (char *)&ts;
	bind[2].is_null = 0;
	bind[2].length = 0;

	bind[3].buffer_type = MYSQL_TYPE_DATETIME;
	bind[3].buffer = (char *)&ts;
	bind[3].is_null = 0;
	bind[3].length = 0;


	ts.year = 2002;
	ts.month = 02;
	ts.day = 03;

	ts.hour=23;
	ts.minute = 45;
	ts.second = 20;

	if(mysql_stmt_bind_param(stmt,bind)){
		perror("mysql_stmt_bind_param error!\n");
		exit(0);
	}

	if(mysql_stmt_execute(stmt)) {
		perror("mysql_stmt_execute error!\n");
		exit(0);
	}

	mysql_stmt_close(stmt);
	//关闭数据库连接
	mysql_close(mysql);

	//释放数据库内容
	mysql_library_end();
	return 0;
}
