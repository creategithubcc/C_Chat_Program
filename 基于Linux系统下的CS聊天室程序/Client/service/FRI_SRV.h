#ifndef _FRIENDS_SRV_H
#define _FRIENDS_SRV_H
#include "../src/List.h"
#include "../src/JSON.h"
#include "./CONNECT.h"
typedef struct friends
{
    int uid;
    char name[30];
    int sex;
    int is_vip;
    int is_online;
    int is_follow;
    int state;
    int NewMsgNum;
    struct friends *next;
} friends_t;
int Fri_Srv_Send_Add(const char *fname);
int Friends_Srv_Recv_Add(const char *cJSON);
int Fri_Srv_Get_List();
int Fri_Srv_App(int uid ,int fuid ,int is_agree);
int Friends_Srv_App_Res(const char *cJSON);
#endif