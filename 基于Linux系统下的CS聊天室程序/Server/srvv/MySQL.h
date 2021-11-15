#ifndef _MYSQL_H
#define _MYSQL_H
#include<mysql/mysql.h>
int MYSQL_CONNECT(const char *host ,const char *user ,const char *pass ,const char *database);
void MySQL_Close();
#endif