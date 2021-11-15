#ifndef _GROUP_SRV_H
#define _GROUP_SRV_H
typedef struct group_t 
{
    int gid;
    char name[30];
    int owner;
    int num;
    int NewMsgNum;
    struct group_t *next;
} group_t;
int Gro_Srv_Cre(const char *gname);
int Gro_Srv_Add_Mem(int gid ,int uid);
int Gro_Srv_Get_List();
void Group_Srv_Join(const char *massage);
void Gro_Srv_Show_Mem(const char *massage);
void Group_Srv_Get_Member(int gid);
void Gro_Srv_Quit(group_t *curGroup);
void Group_Srv_Del(const char *massage);
#endif