#include <stdio.h>
#include "MySQL.h"
MYSQL * mysql;
int MYSQL_CONNECT(const char *host ,const char *user ,const char *pass ,const char *database)
{
    mysql = mysql_init(NULL);
    if(mysql_real_connect(mysql,host,user,pass,database,0,NULL,0) == 0)
    {
        return 0;
    }
    return 1;
}
void MySQL_Close()
{
    mysql_close(mysql);
}