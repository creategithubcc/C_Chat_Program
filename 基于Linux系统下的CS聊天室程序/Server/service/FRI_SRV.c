#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include "ACC_SRV.h"
#include "FRI_SRV.h"
#include "CHAT_SRV.h"
#include "../srvv/ACC_PER.h"
#include "../srvv/FRI_PER.h"
#include "../src/List.h"
#include "../src/JSON.h"
#define MSG_LEN 1024
extern online_t* Onl_List;
int Fri_Srv_Get_List(int sockfd ,const char *JSON)
{
    int uid;
    char buf[MSG_LEN];
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    uid = item -> valueint;
    JSON_shanchu(root);
    friends_t *fri_list = NULL;
    List_Init(fri_list ,friends_t);
    Fri_Per_Get_List(fri_list ,uid);
    friends_t *cur_Pos;
    List_For_Each(fri_list ,cur_Pos)
    {
        root = JSON_Cre_Obj();
        item = JSON_Cre_Str("L");
        JSON_Add_Item_To_Obj(root ,"type" ,item);
        item = JSON_Cre_Num(cur_Pos -> uid);
        JSON_Add_Item_To_Obj(root ,"uid" ,item);
        item = JSON_Cre_Str(cur_Pos -> name);
        JSON_Add_Item_To_Obj(root ,"name" ,item);
        item = JSON_Cre_Bool(cur_Pos -> sex);
        JSON_Add_Item_To_Obj(root ,"sex" ,item);
        item = JSON_Cre_Bool(cur_Pos -> is_vip);
        JSON_Add_Item_To_Obj(root ,"is_vip" ,item);
        item = JSON_Cre_Bool(cur_Pos -> is_follow);
        JSON_Add_Item_To_Obj(root ,"is_follow" ,item);
        item = JSON_Cre_Bool(cur_Pos -> is_online);
        JSON_Add_Item_To_Obj(root ,"is_online" ,item);
        item = JSON_Cre_Bool(cur_Pos -> state);
        JSON_Add_Item_To_Obj(root ,"state" ,item);
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        if(send(sockfd ,(void *)out ,MSG_LEN ,0) < 0)
        {
            perror("发送： 客户端响应失败");
            free(out);
            return 0;
        }
        free(out);
    }
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("L");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(0);
    JSON_Add_Item_To_Obj(root ,"uid" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) < 0)
    {
        perror("发送：客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    ACC_SRV_SEND_ONL(uid ,1);
    List_Destroy(fri_list ,friends_t);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Bool(1);
    JSON_Add_Item_To_Obj(root ,"res" ,item);
    out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) < 0)
    {
        perror("接收: 客户端响应失败");
        free(out);
        return 0;
    }
    free(out);
    Chat_Srv_Send_Off_Priva_Msg(uid);
    return 1;
}
int Fri_Srv_Add(int sockfd ,const char *JSON)
{
    cJSON* root = JSON_Par(JSON);
    cJSON* item = JSON_Get_Obj_Item(root ,"uid");
    int uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"fname");
    int fuid = Acct_Per_Is_user_Name(item -> valuestring);
    JSON_shanchu(root);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Bool((fuid != 0));
    JSON_Add_Item_To_Obj(root ,"res",item);
    if(fuid == 0)
    {
        item = JSON_Cre_Str("用户名不存在!");
        JSON_Add_Item_To_Obj(root ,"reason",item);
    }
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) <= 0)
    {
        perror("send");
        return 0;
    }
    free(out);
    Fri_Per_Add(uid ,fuid);
    if(Chat_Srv_Get_Fri_Sock(fuid) == 0)
    {
        return 1;
    } 
    Fri_Srv_Send_Add(uid ,fuid,"A");
    return 1;
}

int Fri_Srv_Send_Add(int uid ,int fuid ,char* type)
{
    int f_sock_fd = -1;
    friends_t * NewFriends = (friends_t *)malloc(sizeof(friends_t));
    if(*type == 'A')
    {
        NewFriends->uid = uid;
        Fri_Per_Get_Fri_Info(NewFriends);
        f_sock_fd = Chat_Srv_Get_Fri_Sock(fuid);
    }
    else
    { 
        NewFriends->uid = fuid;
        Fri_Per_Get_Fri_Info(NewFriends);
        f_sock_fd = Chat_Srv_Get_Fri_Sock(uid);
        NewFriends -> state = 1;
    }
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str(type);
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(NewFriends -> uid);
    JSON_Add_Item_To_Obj(root ,"uid" ,item);
    item = JSON_Cre_Str(NewFriends -> name);
    JSON_Add_Item_To_Obj(root ,"name" ,item);
    item = JSON_Cre_Bool(NewFriends -> sex);
    JSON_Add_Item_To_Obj(root ,"sex" ,item);
    item = JSON_Cre_Bool(NewFriends -> is_vip);
    JSON_Add_Item_To_Obj(root ,"is_vip" ,item);
    item = JSON_Cre_Bool(NewFriends -> is_follow);
    JSON_Add_Item_To_Obj(root ,"is_follow" ,item);
    item = JSON_Cre_Bool(NewFriends -> is_online);
    JSON_Add_Item_To_Obj(root ,"is_online" ,item);
    item = JSON_Cre_Bool(NewFriends -> state);
    JSON_Add_Item_To_Obj(root ,"state" ,item);
    free(NewFriends);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(f_sock_fd ,(void *)out ,MSG_LEN,0) < 0)
    {
        perror("send");
        printf("发给sock_fd = %d 失败\n",f_sock_fd);
        free(out);
        return 0;
    }
    free(out);
    return 1;
}

int Fri_Srv_App(int sockfd ,const char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    int uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"fuid");
    int fuid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_agree");
    int is_agree = item -> valueint;
    int f_sock_fd = Chat_Srv_Get_Fri_Sock(uid);
    Fri_Per_App(uid ,fuid ,is_agree);
    if(is_agree) 
    {
        Fri_Srv_Send_Add(uid ,fuid ,"a");
    }
    else
    {
        friends_t *NewFriends = (friends_t *)malloc(sizeof(friends_t));
        NewFriends -> uid = fuid;
        Fri_Per_Get_Fri_Info(NewFriends);
        item = JSON_Cre_Str(NewFriends -> name);
        JSON_Add_Item_To_Obj(root ,"fname",item);
        char *out = JSON_prt(root);
        JSON_shanchu(root);
        free(NewFriends);
        if(send(f_sock_fd ,(void*)out ,MSG_LEN ,0) <= 0)
        {
            perror("send");
            return 0;
        }
        free(out);
    }
    return 1;
}