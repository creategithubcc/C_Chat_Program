#ifndef _GROUP_SRV_H
#define _GROUP_SRV_H
#include"./FRI_SRV.h"
typedef struct group_t
{
    int gid;
    char name[50];
    int owner;
    int num;
    struct group_t *next;
}group_t;
typedef struct group_member
{
    int gid;
    friends_t user_info;
    int permission;
    struct group_member *next;
}group_member_t;
int Gro_Srv_Add_Mem(int client_fd ,char *cJSON);
int Gro_Srv_Cre(int client_fd ,char *buf);
int Gro_Srv_Get_List(int client_fd ,char *cJSON);
void Gro_Srv_Send_Info(int gid ,int uid);
void Gro_Srv_Show_Mem(int client_fd ,const char *cJSON);
void Gro_Srv_Quit(int client_fd ,const char *cJSON);
#endif