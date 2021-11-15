#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"../src/List.h"
#include"./FRI_SRV.h"
#include"./CONNECT.h"
#include<pthread.h>
#include"../src/Common.h"
#define MSG_LEN 1024
extern int gl_uid;
extern int sockfd;
extern int mutex_srv;
extern char massage[1024];
friends_t *fri_list;
int Fri_Srv_Get_List()
{
    int rtn;
    if(NULL != fri_list)
    {
        List_Destroy(fri_list ,friends_t);
    }
    List_Init(fri_list ,friends_t);
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("G");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root, "uid" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN,0) < 0)
    {
        perror("发送: 请求服务器失败!");
        return 0;
    }
    free(out);
    JSON_shanchu(root);
    friends_t *newNode = NULL;
    while(1)
    {
        cc_Lock();
        root = JSON_Par(massage);
        item = JSON_Get_Obj_Item(root ,"uid");
        if( item -> valueint == 0)
        {
            cc_Unlock();
            break;
        }
        newNode = (friends_t *)malloc(sizeof(friends_t));
        newNode -> uid = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"name");
        strcpy(newNode -> name ,item -> valuestring);
        item = JSON_Get_Obj_Item(root ,"sex");
        newNode -> sex = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"is_vip");
        newNode -> is_vip = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"is_follow");
        newNode -> is_follow = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"is_online");
        newNode -> is_online = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"state");
        newNode -> state = item -> valueint;
        JSON_shanchu(root);
        newNode -> NewMsgNum = 0;
        newNode -> next = NULL;
        List_AddHead(fri_list ,newNode);
        cc_Unlock();
    }
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root,"res");
    int res = item -> valueint;
    if(res == 1)
    {
        rtn = 1;
    }
    else
    {
        item = JSON_Get_Obj_Item(root ,"reason");
        printf("请求失败: %s",item -> valuestring);
        rtn = 0;
    }
    JSON_shanchu(root);
    cc_Unlock();
    return rtn;
}
int Fri_Srv_Send_Add(const char *fname)
{
    int rtn;
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("A");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root, "uid" ,item);
    item = JSON_Cre_Str(fname);
    JSON_Add_Item_To_Obj(root ,"fname" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) < 0)
    {
        perror("发送: 请求服务器失败！");
        return 0;
    }
    free(out);
    JSON_shanchu(root);
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root,"res");
    int res = item -> valueint;
    if(res)
    {
        printf("好友请求发送成功!");
        getchar();
        rtn = 1;
    }
    else
    {
        item = JSON_Get_Obj_Item(root ,"reason");
        printf("请求失败: %s",item -> valuestring);
        getchar();
        rtn = 0;
    }
    JSON_shanchu(root);
    cc_Unlock();
    return rtn;
}
int Friends_Srv_Recv_Add(const char *JSON)
{
    friends_t *newNode;
    newNode = (friends_t *)malloc(sizeof(friends_t));
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"uid");
    newNode -> uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"name");
    strcpy(newNode -> name ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"sex");
    newNode -> sex = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_vip");
    newNode -> is_vip = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_follow");
    newNode -> is_follow = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_online");
    newNode -> is_online = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"state");
    newNode -> state = item -> valueint;
    JSON_shanchu(root);
    newNode -> NewMsgNum = 0;
    newNode -> next = NULL;
    List_AddHead(fri_list ,newNode);
    if(newNode -> state == 0)
    {
        printf("\n%s请求添加你为好友！\n",newNode -> name);
    }
    return 1;
}
int Fri_Srv_App(int uid ,int fuid ,int is_agree)
{
    cJSON * root = JSON_Cre_Obj();
    cJSON * item = JSON_Cre_Str("a");
    JSON_Add_Item_To_Obj(root ,"type",item);
    item = JSON_Cre_Num(uid);
    JSON_Add_Item_To_Obj(root ,"uid" ,item);
    item = JSON_Cre_Num(fuid);
    JSON_Add_Item_To_Obj(root ,"fuid" ,item);
    item = JSON_Cre_Bool(is_agree);
    JSON_Add_Item_To_Obj(root ,"is_agree" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,(void *)out ,MSG_LEN,0) <= 0 )
    {
        perror("send");
        return 0;
    }
    free(out);
    return 1;
}
int Friends_Srv_App_Res(const char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root,"fuid");
    if(NULL == item) 
    {
        item = JSON_Get_Obj_Item(root ,"name");
        printf("\n%s 同意了你的好友请求！\n",item -> valuestring);
        Friends_Srv_Recv_Add(JSON);
        JSON_shanchu(root);
        return 1;
    }
    item = JSON_Get_Obj_Item(root ,"fname");
    printf("%s\n",JSON);
    printf("\n%s 拒绝了你的好友请求！\n",item -> valuestring);
    JSON_shanchu(root);
    return 1;
}