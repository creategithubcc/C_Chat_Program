#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<sys/socket.h>
#include "./GROUP_SRV.h"
#include "../src/List.h"
#include "./CHAT_SRV.h"
#include "../src/JSON.h"
#include "../srvv/GRO_PER.h"
#define MSG_LEN 1024
int Gro_Srv_Add_Mem(int client_fd ,char *JSON)
{
    int gid ,uid;
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"gid");
    gid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"uid");
    uid = item -> valueint;
    JSON_shanchu(root);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    if(Gro_Per_Add_Mem(gid ,uid))
    {
        item = JSON_Cre_Bool(1);
        Gro_Srv_Send_Info(gid ,uid);
    }
    else
    {
        item = JSON_Cre_Bool(0);
    }
    JSON_Add_Item_To_Obj(root ,"res",item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(client_fd ,out ,MSG_LEN,0) <= 0)
    {
        perror("send");
        free(out);
        return 0;
    }
    return 1;
}
void Gro_Srv_Send_Info(int gid ,int uid)
{
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("J");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    group_t *Group_Info  = Gro_Per_Get_Info(gid);
    item = JSON_Cre_Num(Group_Info -> gid);
    JSON_Add_Item_To_Obj(root ,"gid" ,item);
    item = JSON_Cre_Str(Group_Info -> name);
    JSON_Add_Item_To_Obj(root ,"name" ,item);
    item = JSON_Cre_Num(Group_Info -> owner);
    JSON_Add_Item_To_Obj(root ,"owner" ,item);
    item = JSON_Cre_Num(Group_Info -> num);
    JSON_Add_Item_To_Obj(root ,"num" , item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    int f_fd = Chat_Srv_Get_Fri_Sock(uid); 
    if(send(f_fd ,out ,MSG_LEN,0) <= 0)
    {
        perror("send");
        free(out);
        return ;
    }
    free(out);
}
int Gro_Srv_Cre(int client_fd ,char *buf)
{
    char gname[30];
    int uid;
    cJSON *root = JSON_Par(buf);
    cJSON *item = JSON_Get_Obj_Item(root ,"gname");
    strcpy(gname ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"uid");
    uid = item -> valueint;
    JSON_shanchu(root);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    if(Gro_Per_Is_Gro(gname))
    {
        item = JSON_Cre_Bool(0);
        JSON_Add_Item_To_Obj(root ,"res",item);
        item = JSON_Cre_Str("群已存在");
        JSON_Add_Item_To_Obj(root ,"reason" ,item);
        char *out = JSON_prt(root);
        if(send(client_fd ,out ,MSG_LEN ,0) <= 0)
        {
            perror("send");
            free(out);
            return 0;
        }
        free(out);
        return 0;
    }
    Gro_Srv_Send_Info(Gro_Per_Cre(uid ,gname) ,uid);
    item = JSON_Cre_Bool(1);
    JSON_Add_Item_To_Obj(root , "res",item);
    char *out = JSON_prt(root);
    if(send(client_fd ,out ,MSG_LEN ,0) <= 0)
    {
        perror("send");
        free(out);
        return 0;
    }
    free(out);
    return 1;
}
int Gro_Srv_Get_List(int client_fd ,char *JSON)
{
    int uid;
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    uid = item -> valueint;
    JSON_shanchu(root);
    group_t *group_list = NULL;
    List_Init(group_list ,group_t);
    Gro_Per_Get_Gro_a(group_list ,uid);
    group_t *cur_Pos;
    List_For_Each(group_list ,cur_Pos)
    {
        root = JSON_Cre_Obj();
        item = JSON_Cre_Str("l");
        JSON_Add_Item_To_Obj(root ,"type" ,item);
        item = JSON_Cre_Num(cur_Pos -> gid);
        JSON_Add_Item_To_Obj(root ,"gid" ,item);
        item = JSON_Cre_Str(cur_Pos -> name);
        JSON_Add_Item_To_Obj(root ,"name" ,item);
        item = JSON_Cre_Num(cur_Pos -> owner);
        JSON_Add_Item_To_Obj(root ,"owner" ,item);
        item = JSON_Cre_Num(cur_Pos -> num);
        JSON_Add_Item_To_Obj(root ,"num",item);
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        if(send(client_fd ,(void *)out ,MSG_LEN ,0) < 0)
        {
            perror("发送： 客户端响应失败");
            free(out);
            return 0;
        }
        free(out);
    }
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("l");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(0);
    JSON_Add_Item_To_Obj(root ,"gid" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(client_fd ,(void *)out ,MSG_LEN ,0) < 0)
    {
        perror("发送： 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    List_Destroy(group_list ,group_t);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Bool(1);
    JSON_Add_Item_To_Obj(root ,"res" ,item);
    out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(client_fd ,(void *)out ,MSG_LEN ,0) < 0)
    {
        perror("接收: 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    return 1;
}
void Gro_Srv_Show_Mem(int client_fd ,const char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"gid");
    int gid = item -> valueint;
    JSON_shanchu(root);
    group_member_t *GroupMember;
    List_Init(GroupMember ,group_member_t);
    Gro_Per_Get_Gro_Mem(GroupMember ,gid);
    group_member_t *m;
    List_For_Each(GroupMember ,m)
    {
        root = JSON_Cre_Obj();  
        cJSON_AddStringToObject(root ,"type" ,"m");
        cJSON_AddNumberToObject(root ,"gid",m -> gid);
        cJSON_AddNumberToObject(root ,"uid",m -> user_info.uid);
        cJSON_AddStringToObject(root ,"name" ,m -> user_info.name);
        cJSON_AddNumberToObject(root ,"sex" ,m -> user_info.sex);
        cJSON_AddNumberToObject(root ,"is_vip" , m-> user_info.is_vip);
        cJSON_AddNumberToObject(root ,"is_online" ,m -> user_info.is_online);
        cJSON_AddNumberToObject(root ,"permission" ,m -> permission);
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        if(send(client_fd ,(void *)out ,MSG_LEN ,0) < 0)
        {
            perror("接收: 客户端响应失败");
            free(out);
            continue;
        }
    free(out);
    }
}

void Gro_Srv_Quit(int client_fd ,const char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"do");
    char *_do = item -> valuestring;
    item = JSON_Get_Obj_Item(root ,"gid");
    int gid = item -> valueint;
    int f_fd = 0;
    if(strcmp(_do ,"解散") == 0)
    {
        group_member_t *GroupMember;
        List_Init(GroupMember ,group_member_t);
        Gro_Per_Get_Gro_Mem(GroupMember ,gid);
        group_member_t *m;
        root = JSON_Cre_Obj();
        cJSON_AddStringToObject(root ,"type" ,"D");
        cJSON_AddNumberToObject(root ,"gid" ,gid);
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        List_For_Each(GroupMember ,m)
        {
            if(m -> permission == 2)
            {
                continue;
            } 
            if((f_fd = Chat_Srv_Get_Fri_Sock(m -> user_info.uid)) > 0)
            {
                if(send(f_fd ,out ,MSG_LEN,0) <= 0)
                {
                    perror("send");
                    free(out);
                }
            }
        }
        Gro_Per_Del(gid);
    }
    else 
    {
        item = JSON_Get_Obj_Item(root ,"uid");
        Gro_Per_Del_Mem(gid ,item ->valueint);
    }
}