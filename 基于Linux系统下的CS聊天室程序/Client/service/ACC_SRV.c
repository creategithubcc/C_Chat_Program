#include "./CONNECT.h"
#include "./FRI_SRV.h"
#include "ACC_SRV.h"
#include "../src/List.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
extern char massage[1024];
extern friends_t *fri_list;
extern int mutex_srv;
extern int gl_uid;
extern int sockfd;
void Acc_Srv_Rec_Onl(char *JSON)
{
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"is_online");
    int is_online = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"fuid");
    int fuid = item -> valueint;
    friends_t *f;
    List_For_Each(fri_list ,f)
    {
        if(f -> uid == fuid) 
        {
            f -> is_online = is_online;
            if(is_online)
            {
                printf("\n%s 已上线!\n" ,f -> name);
            }
            else
            {
                printf("\n%s 已下线!\n" ,f -> name);
            }
        }
    }
}
int Acc_Srv_Out(int uid)
{
    int rtn;
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("O");
    JSON_Add_Item_To_Obj(root ,"type",item);
    item = JSON_Cre_Num(uid);
    JSON_Add_Item_To_Obj(root , "uid" ,item);
    char *out = JSON_prt(root);

    if(send(sockfd ,(void *)out ,1024 ,0) <= 0)
    {
        perror("发送请求服务器失败！");
        rtn = 0;
    }
    gl_uid = 0;
    JSON_shanchu(root);
    free(out);
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root ,"res");
    if(item -> valueint == 0)
    {
        item = JSON_Get_Obj_Item(root ,"reason");
        printf("注销失败: %s",item -> valuestring);
        rtn = 0;
    }
    else
    {
        printf("注销成功,按任意键继续!");
        rtn = 1;
    }
    JSON_shanchu(root);
    cc_Unlock();
    getchar();
    return rtn;
}
int Acc_Srv_sig(const char * name ,int sex ,const char * password)
{
    int rtn;
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("S");
    JSON_Add_Item_To_Obj(root,"type",item);
    item = JSON_Cre_Str(name);
    JSON_Add_Item_To_Obj(root,"name",item);
    item = JSON_Cre_Bool(sex);
    JSON_Add_Item_To_Obj(root ,"sex" ,item);
    item = JSON_Cre_Str(password);
    JSON_Add_Item_To_Obj(root,"password",item);
    char *out = JSON_prt(root);

    if(send(sockfd , (void *)out , 1024 ,0) < 0 )
    {
        perror("发送: 请求服务器失败");
        return 0;
    }
    free(out);
    JSON_shanchu(root);
    cc_Lock();
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root,"res");
    int res = item -> valueint;
    if(res == 1) 
    {
        printf("注册成功!按任意键继续!");
        getchar();
        rtn = 1;
    }
    else
    {
        item = JSON_Get_Obj_Item(root,"reason");
        printf("注册失败: %s",item -> valuestring);
        getchar();
        rtn = 0;
    }
    JSON_shanchu(root);
    cc_Unlock();
    return rtn;
}
int Acc_Srv_Log(const char *name , const char *password)
{
    int rtn;
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("L");
    JSON_Add_Item_To_Obj(root,"type",item);
    item = JSON_Cre_Str(name);
    JSON_Add_Item_To_Obj(root,"name",item);
    item = JSON_Cre_Str(password);
    JSON_Add_Item_To_Obj(root,"password",item);
    char *out = JSON_prt(root);

    if(send(sockfd , out , 1024 ,0) < 0 )
    {
        perror("发送: 请求服务器失败!");
        return 0;
    }
    free(out);
    cc_Lock();
    JSON_shanchu(root);
    root = JSON_Par(massage);
    item = JSON_Get_Obj_Item(root,"res");
    int res = item -> valueint;
    if(res == 1) 
    {
        item = JSON_Get_Obj_Item(root,"uid");
        rtn = item -> valueint;
        printf("登录成功!请稍候..");
        fflush(stdout);
        sleep(2);
    }
    else
    {
        item = JSON_Get_Obj_Item(root,"reason");
        rtn = 0;
        printf("登录失败: %s",item -> valuestring);
        getchar();
    }
    JSON_shanchu(root);
    cc_Unlock();
    return rtn;
}