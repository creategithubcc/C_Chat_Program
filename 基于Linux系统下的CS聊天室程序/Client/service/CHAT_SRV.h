#ifndef _CHAT_SRV_H
#define _CHAT_SRV_H
#include "../src/Common.h"
typedef struct private_msg 
{
    int from_uid;
    char name[30];
    char msg[1000];
    char time [25];
    struct private_msg *next;
}private_msg_t;
typedef struct group_msg 
{
    int from_uid;
    int gid;
    char uname[30];
    char gname[30];
    char msg[1000];
    char time [25];
    struct group_msg *next;
}group_msg_t;
void Chat_Srv_Init_List();
void Chat_Srv_Rec_Priva(const char *cJSON);
int Chat_Srv_Send_Priva(int to_uid ,const char *msg);
void Chat_Srv_Rec_Group(const char *cJSON);
int Chat_Srv_Send_Group(int to_uid ,const char *msg);
int Chat_Srv_Send_File(const char *filename ,int fuid);
int Chat_Srv_Rec_File(const char *cJSON);
void Chat_Srv_Show_Priva_Rec2(const char *cJSON);
void Chat_Srv_Get_Priva_Rec(int fuid);
#endif