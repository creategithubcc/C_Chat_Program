#ifndef _FRIENDS_SRV_H
#define _FRIENDS_SRV_H
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
    struct friends *next;
} friends_t;
int Fri_Srv_Get_List(int sockfd ,const char *cJSON);
int Fri_Srv_Add(int sockfd ,const char *cJSON);
int Fri_Srv_Send_Add(int uid,int fuid ,char* type);
int Friends_Srv_Del(int sockfd ,const char *cJSON);
int Fri_Srv_App(int sockfd ,const char *cJSON); 
#endif