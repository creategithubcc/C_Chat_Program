#ifndef _CHAT_PERSIST_H
#define _CHAT_PERSIST_H
#include<mysql/mysql.h>
int Chat_Per_Gro(int uid ,int gid ,const char *msg ,const char *offlist);
int Chat_Per_Priva(int from_uid ,int  to_uid ,const char *msg ,int is_offline);
MYSQL_RES *Chat_Per_Get_Priva_Rec(int uid ,int fuid);
MYSQL_RES* Chat_Per_Get_Offline_Priva_Msg(int uid);
#endif