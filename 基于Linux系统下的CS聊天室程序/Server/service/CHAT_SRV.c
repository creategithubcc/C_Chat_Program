#include<stdio.h>
#include<string.h>
#include<mysql/mysql.h>
#include"./ACC_SRV.h"
#include"./CONNECT.h"
#include"./CHAT_SRV.h"
#include"./GROUP_SRV.h"
#include"../src/JSON.h"
#include"../srvv/CHAT_PER.h"
#include"../srvv/GRO_PER.h"
#include"../srvv/ACC_PER.h"
#include"../src/Common.h"
#include"../src/List.h"
#define MSG_LEN 1024
extern online_t *Onl_List;
void Chat_Srv_File(const char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"fuid");
    int fuid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"size");
    item = JSON_Get_Obj_Item(root,"con");
    int f_sock_fd = -1;
    online_t *o;
    List_For_Each(Onl_List ,o)
    {
        if(o -> uid == fuid)
        {
            f_sock_fd = o-> sockfd;
            break;
        }
    }
    if(f_sock_fd == -1)
    {
        return ;
    } 
    if(send(f_sock_fd ,JSON ,MSG_LEN,0) <= 0)
    {
        perror("send");
        JSON_shanchu(root);
        return ;
    }
    JSON_shanchu(root);
    return ;
}
int Chat_Srv_Get_Fri_Sock(int fuid)
{
    online_t *cur_Pos;
    int to_sock = -1;
    List_For_Each(Onl_List ,cur_Pos)
    {
        if(cur_Pos -> uid == fuid)
        {
            to_sock = cur_Pos -> sockfd;
        }
    }
    return to_sock;
}
int Chat_Srv_Priva2(int sockfd ,const char *JSON)
{
    int from_uid ,to_uid ,to_sock ;
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",Srvdate.year ,Srvdate.month ,Srvdate.day,Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"from_uid");
    from_uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"to_uid");
    to_uid = item -> valueint;
    item = JSON_Cre_Str(Srvdatetime);
    JSON_Add_Item_To_Obj(root ,"time" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    to_sock = Chat_Srv_Get_Fri_Sock(to_uid);
    Chat_Per_Priva(from_uid ,to_uid ,out ,(to_sock == -1));
    if(to_sock == -1)
    {
        return 2;
    } 
    if(send(to_sock ,(void *)out ,MSG_LEN ,0) <= 0)
    {
        perror("send:");
        free(out);
        return 0;
    }
    free(out);
    return 1;
}
int Chat_Srv_Group(int sockfd ,const char *JSON)
{
    int from_uid ,to_gid ,to_sock ;
    char offlist[100] = "," ,str[4];
    group_member_t *GroupMember ,*g;
    List_Init(GroupMember ,group_member_t);
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",Srvdate.year ,Srvdate.month ,Srvdate.day,Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"from_uid");
    from_uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"to_gid");
    to_gid = item -> valueint;
    item = JSON_Cre_Str(Srvdatetime);
    JSON_Add_Item_To_Obj(root ,"time" ,item);
    item = JSON_Cre_Str(Acc_Per_Get_User_Name_From_Uid(from_uid));
    JSON_Add_Item_To_Obj(root ,"uname", item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    Gro_Per_Get_Gro_Mem(GroupMember ,to_gid);
    List_For_Each(GroupMember ,g)
    {
        if(g -> user_info.uid == from_uid)
        {
            continue;
        } 
        to_sock = Chat_Srv_Get_Fri_Sock(g -> user_info.uid);
        if(to_sock == -1) 
        {
            sprintf(str ,"%d,",g->user_info.uid);
            strcat(offlist ,str);
            continue;
        }
        if(send(to_sock ,(void *)out ,MSG_LEN ,0) <= 0)
        {
            perror("send:");
            free(out);
            return 0;
        }
    }
    Chat_Per_Gro(from_uid ,to_gid ,out ,offlist);
    free(out);
    return 1;
}
void Chat_Srv_Send_Off_Priva_Msg(int uid)
{
    MYSQL_RES *res = Chat_Per_Get_Offline_Priva_Msg(uid);
    if(res == NULL)
    {
        return;
    } 
    MYSQL_ROW row;
    int to_sock = Chat_Srv_Get_Fri_Sock(uid);
    while((row = mysql_fetch_row(res)))
    {
        if(send(to_sock ,row[0] ,MSG_LEN ,0) <= 0)
        {
            perror("send:");
            continue;
        }
    }
    mysql_free_result(res);
}
void Chat_Srv_Send_Priva_Res(int sockfd ,const char *JSON)
{
    MYSQL_RES *res = NULL;
    MYSQL_ROW row;
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    int uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"fuid");
    int fuid = item -> valueint;
    JSON_shanchu(root);
    res = Chat_Per_Get_Priva_Rec(uid ,fuid);
    while((row = mysql_fetch_row(res)))
    {
        root = JSON_Par(row[0]);
        item = JSON_Get_Obj_Item(root ,"type");
        strcpy(item -> valuestring,"E");
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        if(send(sockfd ,(void *)out ,MSG_LEN ,0) <= 0)
        {
            perror("send:");
            free(out);
            continue;
        }
        free(out);
    }
    mysql_free_result(res);
}