#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "./ACC_SRV.h"
#include "./CHAT_SRV.h"
#include "./FRI_SRV.h"
#include "../srvv/ACC_PER.h"
#include "../srvv/FRI_PER.h"
#include "../src/JSON.h"
#include "../src/List.h"
#define MSG_LEN 1024
extern online_t * Onl_List;

int ACC_SRV_SEND_ONL(int uid ,int is_online)
{
    int f_sock_fd;
    friends_t *fri_list ,*f;
    List_Init(fri_list ,friends_t);
    Fri_Per_Get_List(fri_list ,uid);
    List_For_Each(fri_list ,f)
    {
        if(f->is_online)
        {
            f_sock_fd = Chat_Srv_Get_Fri_Sock(f->uid);
            if(f_sock_fd == -1)
            {
                return 0;
            } 
            cJSON *root = JSON_Cre_Obj();
            cJSON *item = JSON_Cre_Str("I");
            JSON_Add_Item_To_Obj(root ,"type" ,item);
            item = JSON_Cre_Num(uid);
            JSON_Add_Item_To_Obj(root ,"fuid",item);
            item = JSON_Cre_Bool(is_online);
            JSON_Add_Item_To_Obj(root ,"is_online",item);
            char *out = JSON_prt(root);
            JSON_shanchu(root);
            if(send(f_sock_fd ,(void *)out ,MSG_LEN ,0) <= 0)
            {
                perror("send 客户端响应失败");
                free(out);
                return 0;
            }
            free(out);
        }
    }
    List_Destroy(fri_list, friends_t);
    return 1;
}
int Acc_Srv_Ch_Onl(int uid ,int is_online ,int sockfd)
{
    int rtn = 0;
    online_t *cur_Pos;
    if(is_online)
    {
        List_For_Each(Onl_List ,cur_Pos)
        {
            if(cur_Pos -> uid == uid)
            {
                close(cur_Pos -> sockfd);
                cur_Pos -> sockfd = sockfd;
                rtn = 1;
                goto per;
            }
        }
        cur_Pos = (online_t *)malloc(sizeof(online_t));
        cur_Pos -> uid = uid;
        cur_Pos -> sockfd = sockfd;
        cur_Pos -> next = NULL;
        List_AddHead(Onl_List ,cur_Pos);
        rtn = 1;
    }
    else
    {
        List_For_Each(Onl_List ,cur_Pos)
        {
            if(cur_Pos -> sockfd == sockfd)
            {
                uid = rtn = cur_Pos -> uid;
                List_FreeNode(Onl_List ,cur_Pos ,online_t);
                break;
            }
        }
    }
    if(uid == -1)
    {
        return 0;
    } 
    per: if(Acc_Per_Ch_Is_Onl(uid ,is_online) == 0)
    {
        rtn = 0;
    } 
    return rtn;
}
int Acc_Srv_Out(int sockfd ,char *JSON)
{
    int rtn;
    int uid;
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    uid = item -> valueint;
    JSON_shanchu(root);
    rtn = Acc_Srv_Ch_Onl(uid ,0 ,sockfd);
    if(rtn != -1)
    { 
        ACC_SRV_SEND_ONL(uid ,0);
    }
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Bool((rtn > 0));
    JSON_Add_Item_To_Obj(root ,"res",item);
    item = JSON_Cre_Str("服务器异常?");
    JSON_Add_Item_To_Obj(root , "reason" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void*)out , MSG_LEN ,0) <= 0)
    {
        rtn = 0;
    }
    return rtn;
}
int Acc_Srv_sig(int sockfd ,char * JSON)
{
    int sex;
    char name[30],password[30];
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root , "name");
    strcpy(name,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"sex");
    sex = item -> valueint;
    item = JSON_Get_Obj_Item(root , "password");
    strcpy(password , item -> valuestring);
    JSON_shanchu(root);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    if(Acct_Per_Is_user_Name(name))
    {
        item = JSON_Cre_Bool(0);
        JSON_Add_Item_To_Obj(root , "res" , item);
        item = JSON_Cre_Str("用户名已存在!");
        JSON_Add_Item_To_Obj(root , "reason" , item);
        char *out = JSON_prt(root);
        if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
        JSON_shanchu(root);
        free(out);
    }
    else
    {
        if(Acc_Per_Add_User(name ,sex ,password))
        {
            item = JSON_Cre_Bool(1);
            JSON_Add_Item_To_Obj(root , "res" , item);
            char *out = JSON_prt(root);
            if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
            JSON_shanchu(root);
            free(out);
            return 1;
        }
        item = JSON_Cre_Bool(0);
        JSON_Add_Item_To_Obj(root , "res" , item);
        item = JSON_Cre_Str("服务器异常!");
        JSON_Add_Item_To_Obj(root , "reason" , item);
        char *out = JSON_prt(root);
        if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
        JSON_shanchu(root);
        free(out);
    }
    return 0;
}
int Acc_Srv_Log(int sockfd ,char *JSON)
{
    int uid;
    char name[30] , password[30];
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root , "name");
    strcpy(name,item -> valuestring);
    item = JSON_Get_Obj_Item(root , "password");
    strcpy(password , item -> valuestring);
    JSON_shanchu(root);
    root = JSON_Cre_Obj();
    item = JSON_Cre_Str("R");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    if((uid = Acct_Per_Is_user_Name(name)) == 0)
    {
        item = JSON_Cre_Bool(0);
        JSON_Add_Item_To_Obj(root , "res" , item);
        item = JSON_Cre_Str("用户名不存在");
        JSON_Add_Item_To_Obj(root , "reason" , item);
        char *out = JSON_prt(root);
        if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
        JSON_shanchu(root);
        free(out);
    }
    else
    {
        if(Acc_Per_Mat_User_And_Password(uid ,password))
        {
            Acc_Srv_Ch_Onl(uid ,1 ,sockfd);
            item = JSON_Cre_Bool(1);
            JSON_Add_Item_To_Obj(root , "res" , item);
            item = JSON_Cre_Num(uid);
            JSON_Add_Item_To_Obj(root , "uid" ,item);
            char *out = JSON_prt(root);
            if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
            JSON_shanchu(root);
            free(out);
            return 1;
        }
        item = JSON_Cre_Bool(0);
        JSON_Add_Item_To_Obj(root , "res" , item);
        item = JSON_Cre_Str("用户名密码不匹配!");
        JSON_Add_Item_To_Obj(root , "reason" , item);
        char *out = JSON_prt(root);
        if(send(sockfd , (void *)out , MSG_LEN , 0) < 0){}
        JSON_shanchu(root);
        free(out);
    }
    return 0;
}