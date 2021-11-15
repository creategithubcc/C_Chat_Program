#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "ACC_PER.h"
#include "MySQL.h"
#include "../service/FRI_SRV.h"
extern MYSQL * mysql;
int Acc_Per_Ch_Is_Onl(int uid ,int is_online)
{
    char SQL[100];
    sprintf(SQL ,"UPDATE account SET is_online = '%d' WHERE uid = '%d'" ,is_online ,uid);
    if(mysql_real_query(mysql , SQL , strlen(SQL)))
    {
        printf("%s\n",mysql_error(mysql));
        return 0;
    }
    return 1;
}
int Acct_Per_Is_user_Name(const char * name)
{
    int rtn = 0;
    char SQL[100];
    MYSQL_RES * res;
    MYSQL_ROW row;
    sprintf(SQL,"SELECT uid FROM account WHERE name = '%s'",name);
    if(mysql_real_query(mysql , SQL , strlen(SQL)))
    {
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(row)
    {
        rtn = atoi(row[0]);
    } 
    mysql_free_result(res);
    return rtn;
}
int Acc_Per_Add_User(const char *name ,int sex , const char *password)
{
    char SQL[100];
    sprintf(SQL,"INSERT INTO account VALUES (NULL , '%s' ,'%d' , 0, 0 , md5('%s'))", name ,sex , password);
    if(mysql_real_query(mysql , SQL , strlen(SQL)))
    {
        printf("%s\n",mysql_error(mysql));
        return 0;
    }
    return 1;
}
int Acc_Per_Mat_User_And_Password(int uid , const char * password)
{
    int rtn;
    char SQL[100];
    MYSQL_RES * res;
    MYSQL_ROW row;
    sprintf(SQL,"SELECT * FROM account WHERE (uid = '%d' AND password = md5('%s'))" , uid , password);
    if(mysql_real_query(mysql , SQL ,strlen(SQL)))
    {
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(row)
    {
        rtn = 1;
    } 
    else
    {
        rtn = 0;
    } 
    mysql_free_result(res);
    return rtn;
}
char * Acc_Per_Get_User_Name_From_Uid(int uid)
{
    MYSQL_RES *res;
    MYSQL_ROW row;
    char * rtn = NULL;
    char SQL[100];
    sprintf(SQL, "SELECT name FROM account WHERE uid = '%d'" ,uid);
    if(mysql_real_query(mysql , SQL ,strlen(SQL)))
    {
        printf("%s\n" ,mysql_error(mysql));
        return 0;
    }
    res = mysql_store_result(mysql);
    row = mysql_fetch_row(res);
    if(row)
    {
        rtn = (char *)malloc(sizeof(char) * 30);
        strcpy(rtn ,row[0]);
    }
    mysql_free_result(res);
    return rtn;
}