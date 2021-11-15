#ifndef _FRIENDS_PERSIST_H
#define _FRIENDS_PERSIST_H
#include"../service/FRI_SRV.h"
#include<mysql/mysql.h>
int Fri_Per_Add(int uid ,int fuid);
int Fri_Per_Get_List(friends_t * fri_list ,int uid);
int Fri_Per_App(int uid ,int fuid ,int is_argee);
int Fri_Per_Get_Fri_Info(friends_t * Node );
#endif