#ifndef _GROUP_PERSIST_H
#define _GROUP_PERSIST_H
#include "../service/GROUP_SRV.h"
int Gro_Per_Is_Gro(const char *gname);
int Gro_Per_Cre(int uid ,const char *name);
int Gro_Per_Add_Mem(int gid ,int uid);
int Gro_Per_Del_Mem(int gid ,int uid);
int Gro_Per_Del(int gid);
int Gro_Per_Get_Gro_a(group_t *MyGroupList ,int uid);
int Gro_Per_Get_Gro_Mem(group_member_t* GroupMember,int gid);
group_t * Gro_Per_Get_Info(int gid);
#endif