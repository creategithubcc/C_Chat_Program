#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"./CHAT_PER.h"
#include<mysql/mysql.h>
extern MYSQL *mysql;
int Chat_Per_Priva(int from_uid ,int to_uid ,const char *msg, int is_offline)
{
    char SQL[100 + strlen(msg)];
    sprintf(SQL , "INSERT INTO private_rec VALUES('%d' ,'%d' ,'%s','%d')" ,from_uid ,to_uid ,msg  ,is_offline);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL)))
    {
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}
int Chat_Per_Gro(int uid ,int gid ,const char *msg, const char *offlist)
{
    char SQL[100 + strlen(msg)];
    sprintf(SQL , "INSERT INTO group_rec VALUES('%d' ,'%d' ,'%s','%s')" ,gid ,uid ,msg ,offlist);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL)))
    {
        printf("%s",mysql_error(mysql));
        return 0;
    }
    return 1;
}
MYSQL_RES * Chat_Per_Get_Offline_Priva_Msg(int uid)
{
    char SQL[100];
    MYSQL_RES *res;
    sprintf(SQL,"SELECT msg FROM private_rec WHERE (to_uid = '%d' AND is_offline = 1)" ,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL)))
    {
        printf("%s\n",mysql_error(mysql));
        return NULL;
    }
    res = mysql_store_result(mysql);
    sprintf(SQL, "UPDATE private_rec set is_offline = '0' WHERE to_uid = '%d'" ,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL)))
    {
        printf("%s\n",mysql_error(mysql));
        return NULL;
    }
    return res;
}
MYSQL_RES *Chat_Per_Get_Priva_Rec(int uid ,int fuid)
{
    char SQL[200];
    sprintf(SQL ,"SELECT msg FROM private_rec WHERE ((to_uid = '%d' AND from_uid = '%d')" " OR (to_uid = '%d' AND from_uid = '%d'))" ,uid ,fuid, fuid ,uid);
    if(mysql_real_query(mysql ,SQL ,strlen(SQL)))
    {
        printf("%s\n",mysql_error(mysql));
        return NULL;
    }
    return mysql_store_result(mysql);
}