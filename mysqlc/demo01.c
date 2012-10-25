#include <stdio.h>

#include <stdlib.h>

#include <stdarg.h>

#include "mysql.h"

 

MYSQL *mysql;

MYSQL_RES *results;

MYSQL_ROW record;

 

static char *server_options[] = { "mysql_test", "--defaults-file=my.cnf" };

int num_elements = sizeof(server_options)/ sizeof(char *);

 

static char *server_groups[] = { "libmysqld_server", "libmysqld_client" };

 

int main(int argc,char *argv[])

{

   //mysql_server_init(num_elements, server_options, server_groups);

   mysql = mysql_init(NULL);

   //mysql_options(mysql, MYSQL_READ_DEFAULT_GROUP, "libmysqld_client");

   //mysql_options(mysql, MYSQL_OPT_USE_EMBEDDED_CONNECTION, NULL);

 

   mysql_real_connect(mysql, NULL,"root","goodbaby", "mysql", 0,NULL,0);

 

   mysql_query(mysql, "SELECT host,user,password from user");

 

   results = mysql_store_result(mysql);

 

   while((record = mysql_fetch_row(results))) {

      printf("%s - %s \n", record[0], record[1]);

   }

 

   mysql_free_result(results);

   mysql_close(mysql);

   //mysql_server_end();
   mysql_library_end();
 

   exit(0);

}
