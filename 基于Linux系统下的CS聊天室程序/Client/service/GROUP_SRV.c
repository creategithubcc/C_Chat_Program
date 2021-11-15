#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<sys/socket.h>
#include"../src/JSON.h"
#include"../src/Common.h"
#include"./GROUP_SRV.h"
#include"../src/List.h"
#include"./FRI_SRV.h"
#define MSG_LEN 1024
extern int gl_uid;
extern int sockfd;
extern int mutex_srv;
extern char massage[1024];
extern friends_t *fri_list;
extern group_t *curGroup;
group_t *group_list;
int Gro_Srv_Cre(const char *gname)
{
    int rtn = 0;
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("c");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Str(gname);
    JSON_Add_Item_To_Obj(root ,"gname",item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root, "uid" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    
    if(send(sockfd ,out ,MSG_LEN ,0) <= 0)
    {
        free(out);
        perror("send");
        return 0;
    }
    free(out);
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root ,"res");
    int res = item -> valueint;
    if(res)
    {
        printf("群创建成功!");
        getchar();
        rtn = 1;
    }
    else
    {
        item = JSON_Get_Obj_Item(root ,"reason");
        printf("创建失败 :%s ",item -> valuestring);
        getchar();
        rtn = 0;
    }
    JSON_shanchu(root);
    cc_Unlock();
    return rtn;
}
int Gro_Srv_Get_List()
{
    int rtn;
    if(NULL != group_list)
    {
        List_Destroy(group_list ,group_t);
    }
    List_Init(group_list ,group_t);
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("g");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root, "uid" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN,0) < 0)
    {
        perror("send: 请求服务器失败！");
        return 0;
    }
    free(out);
    JSON_shanchu(root);
    group_t *newNode = NULL;
    while(1)
    {
        cc_Lock();
        root = JSON_Par(massage);
        item = JSON_Get_Obj_Item(root ,"gid");
        
        if( item -> valueint == 0)
        {
            cc_Unlock();
            break;
        }
        newNode = (group_t *)malloc(sizeof(group_t));
        newNode -> gid = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"name");
        strcpy(newNode -> name ,item -> valuestring);
        item = JSON_Get_Obj_Item(root ,"owner");
        newNode -> owner = item -> valueint;
        item = JSON_Get_Obj_Item(root ,"num");
        newNode -> num = item -> valueint;
        JSON_shanchu(root);
        newNode -> next = NULL;
        List_AddHead(group_list ,newNode);
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
int Gro_Srv_Add_Mem(int gid ,int uid)
{
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("M");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gid);
    JSON_Add_Item_To_Obj(root ,"gid",item);
    item = JSON_Cre_Num(uid);
    JSON_Add_Item_To_Obj(root , "uid" ,item);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,out ,MSG_LEN ,0) <= 0)
    {
        free(out);
        perror("send");
        return 0;
    }
    free(out);
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root ,"res");
    int res = item -> valueint;
    if(res)
    {
        printf("邀请成功!");
        getchar();
    }
    else
    {
        printf("邀请失败,该成员已在当前群聊中!");
        getchar();
    }
    JSON_shanchu(root);
    cc_Unlock();
    return 1;
}
void Group_Srv_Join(const char * massage)
{
    group_t *newNode = (group_t *)malloc(sizeof(group_t));
    cJSON *root = JSON_Par(massage);
    cJSON *item = JSON_Get_Obj_Item(root,"gid");
    newNode -> gid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"name");
    strcpy(newNode -> name ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"owner");
    newNode -> owner = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"num");
    newNode -> num  = item -> valueint;
    newNode -> NewMsgNum = 0;
    newNode -> next = NULL;
    List_AddHead(group_list ,newNode);
    if(newNode -> owner == gl_uid) 
    {
        return;
    }
    friends_t *f;
    List_For_Each(fri_list ,f)
    {
        if(newNode -> owner == f -> uid)
        {
            printf("\n%s 邀请你加入了群聊！ %s\n",f -> name ,newNode -> name);
            return;
        }
    } 
}
void Gro_Srv_Show_Mem(const char *massage)
{
    cJSON *root ,*item;
    friends_t GroupMember;
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root ,"name");
    strcpy(GroupMember.name ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"sex");
    GroupMember.sex = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_online");
    GroupMember.is_online = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"is_vip");
    GroupMember.is_vip = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"permission");
    char *is_online[2] = {"@","\e[32m@\e[0m"};//qw
    char *is_vip[2] = {"","\e[31m"};
    char *sex[2] = {"\e[35m女\e[0m","\e[36m男\e[0m"};//qw
    char *per[3] ={"" ,"[\e[32m管理员\e[0m]" ,"[\e[33m群主\e[0m]"};
    printf("   %s %s%s\e[0m %s %s\n" ,is_online[GroupMember.is_online] ,is_vip[GroupMember.is_vip],GroupMember.name ,sex[GroupMember.sex],per[item -> valueint]);
    JSON_shanchu(root);
}
void Group_Srv_Get_Member(int gid)
{
    cJSON *root = JSON_Cre_Obj();
    cJSON_AddStringToObject(root ,"type" ,"m");
    cJSON_AddNumberToObject(root ,"gid" ,gid);
    char * out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,out ,MSG_LEN ,0) <= 0)
    {
        perror("send");
    }
    free(out);
}
void Gro_Srv_Quit(group_t *curGroup)
{
    char choice[5];
    cJSON *root = JSON_Cre_Obj();
    cJSON_AddStringToObject(root ,"type" ,"Q");
    cJSON_AddNumberToObject(root ,"gid" ,curGroup -> gid);
    if(curGroup -> owner == gl_uid)
    {
        cJSON_AddStringToObject(root ,"do" ,"解散");
        printf("确认解散群聊 %s ?[yes/no]", curGroup -> name);
    } 
    else
    {
        cJSON_AddStringToObject(root ,"do" ,"退群");
        cJSON_AddNumberToObject(root ,"uid" ,gl_uid);
        printf("确认要退出群聊 %s ?[yes/no]" ,curGroup -> name);
    }
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    sgets(choice ,5);
    if(strcmp(choice ,"yes") != 0) 
    {
        free(out);
        return;
    }
    if(send(sockfd ,out ,MSG_LEN ,0) <= 0)
    {
        perror("send");
    }
    free(out);
    List_FreeNode(group_list ,curGroup ,group_t);
    printf("操作成功！");
    getchar();
}
void Group_Srv_Del(const char *massage)
{
    cJSON *root = JSON_Par(massage);
    cJSON *item = JSON_Get_Obj_Item(root ,"gid");
    int gid = item -> valueint;
    JSON_shanchu(root);
    group_t *g;
    List_For_Each(group_list ,g)
    {
        if(g -> gid == gid)
        {
            printf("\n群主已将群聊 %s 解散!\n",g -> name);
            List_FreeNode(group_list ,g ,group_t);
            curGroup = NULL;
            return;
        }
    }
}