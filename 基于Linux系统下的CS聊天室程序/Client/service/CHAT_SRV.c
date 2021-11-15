#include"../service/FRI_SRV.h"
#include"../service/GROUP_SRV.h"
#include"../src/JSON.h"
#include"./CHAT_SRV.h"
#include"../src/List.h"
#include"./CONNECT.h"
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<libgen.h>
#include<string.h>
#define MSG_LEN 1024
extern int gl_uid;
extern int sockfd;
extern friends_t *fri_list;
extern group_t *group_list;
private_msg_t *Pri_msg_list;
group_msg_t *group_msg_list;
int Chat_Srv_Rec_File(const char *JSON)
{
    char output_cod[650] ,buf[900];
    memset(buf ,0,sizeof(buf));
    char filename [100] = "download/";
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"filename");
    strcat(filename , item->valuestring);
    int fd = open(filename ,O_WRONLY|O_CREAT|O_APPEND,S_IRUSR|S_IWUSR);
    if(fd == -1)
    {
        perror("open");
        return 0;
    }
    item = JSON_Get_Obj_Item(root ,"con");
    strcat( buf , item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"size");
    int size = item -> valueint;
    W64jiema input_sta;
    chushihua(&input_sta);
    jiemakuai(buf ,strlen(buf) ,output_cod ,&input_sta);
    if(write(fd,output_cod ,size) != size)
    {
        perror("write");
        close(fd);
        return 0;
    }
    if(size < 650 - 2) 
    {
        item = JSON_Get_Obj_Item(root ,"uid");
        int uid = item -> valueint;
        friends_t *f;
        List_For_Each(fri_list ,f)
        {
            if(f -> uid == uid)
            {
                printf("\n%s 发来一个文件,已保存至./download/%s\n",f->name ,basename(filename));
                break;
            }
        }
    }
    close(fd);
    return 0;
}
int Chat_Srv_Send_File(const char *filename ,int fuid)
{
    char buf[650] ,output_cod[900] ,code_end[5],*out;
    int fd ,size;
    W64bianmazhuangtai input_sta;
    if((fd = open(filename ,O_RDONLY)) == -1)
    {
        printf("读取权限不够or文件不存在!");
        return 0;
    }
    while(1)
    {
        memset(buf ,0,sizeof(buf));
        size = read(fd ,buf ,sizeof(buf) - 2);
        chushihua2(&input_sta);
        memset(output_cod ,0,sizeof(output_cod));
        bianmakuai(buf ,size ,output_cod ,&input_sta);
        if(input_sta.step != step_a) 
        {
            memset(code_end ,0,sizeof(code_end));
            bianmakuai2(code_end ,&input_sta);
            strcat(output_cod ,code_end);
        }
        cJSON *root = JSON_Cre_Obj();
        cJSON *item = JSON_Cre_Str("F");
        JSON_Add_Item_To_Obj(root ,"type" ,item);
        item = JSON_Cre_Num(gl_uid);
        JSON_Add_Item_To_Obj(root ,"uid",item);
        item = JSON_Cre_Num(fuid);
        JSON_Add_Item_To_Obj(root ,"fuid" ,item);
        item = JSON_Cre_Str(basename((char*)filename));
        JSON_Add_Item_To_Obj(root ,"filename" ,item);
        item = JSON_Cre_Num(size);
        JSON_Add_Item_To_Obj(root ,"size" ,item);
        item = JSON_Cre_Str(output_cod);
        JSON_Add_Item_To_Obj(root ,"con",item);
        out = JSON_prt(root);
        JSON_shanchu(root);
        int ret;
	    printf("%s\n",out);
        if((ret = send(sockfd ,out,MSG_LEN ,0)) <= 0)
        {
            perror("send");
            free(out);
            return 0;
        }
        if(size < (int)sizeof(buf) - 2) 
        {
            break;
        }
        free(out);
    }
    close(fd);
    return 1;
}
void Chat_Srv_Init_List()
{
    List_Init(Pri_msg_list ,private_msg_t);
    List_Init(group_msg_list ,group_msg_t);
}
void Chat_Srv_Rec_Priva(const char *JSON)
{
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    friends_t * f;
    List_For_Each(fri_list ,f)
    {
        if(f->uid == NewMsg->from_uid)
        {
            strcpy(NewMsg->name ,f->name);
            (f->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    JSON_shanchu(root);
    private_msg_t *cur_Pos;
    List_AddTail(Pri_msg_list ,cur_Pos ,NewMsg);
    printf("\n%s 发来一条消息!\n",NewMsg->name);
}

void Chat_Srv_Rec_Group(const char *JSON)
{
    group_msg_t *NewMsg = (group_msg_t *)malloc(sizeof(group_msg_t));
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"to_gid");
    NewMsg -> gid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    item = JSON_Get_Obj_Item(root,"uname");
    strcpy(NewMsg -> uname ,item -> valuestring);
    group_t * g;
    List_For_Each(group_list ,g)
    {
        if(g->gid == NewMsg->gid)
        {
            strcpy(NewMsg->gname ,g->name);
            (g->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    JSON_shanchu(root);
    group_msg_t *cur_Pos;
    List_AddTail(group_msg_list ,cur_Pos ,NewMsg);
    printf("\n你的群聊 %s 发来了一条新消息!\n",NewMsg->gname);
}
int Chat_Srv_Send_Priva(int to_uid ,const char * msg)
{
    int rtn = 1;
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",Srvdate.year ,Srvdate.month ,Srvdate.day,Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    strcpy(NewMsg->time ,Srvdatetime);
    NewMsg->from_uid = gl_uid;
    strcpy(NewMsg->msg ,msg);
    NewMsg -> next = NULL;
    private_msg_t *m;
    List_AddTail(Pri_msg_list ,m ,NewMsg);   
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("P"); 
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root ,"from_uid" ,item);
    item = JSON_Cre_Num(to_uid);
    JSON_Add_Item_To_Obj(root ,"to_uid" ,item);
    item = JSON_Cre_Str(msg);
    JSON_Add_Item_To_Obj(root ,"msg" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) <= 0)
    {
        printf("服务器失去响应!\n");
        rtn = 0;
    }
    JSON_shanchu(root);
    free(out);
    return rtn;
}
int Chat_Srv_Send_Group(int to_gid ,const char * msg)
{
    int rtn = 1;
    group_msg_t *NewMsg = (group_msg_t *)malloc(sizeof(group_msg_t));
    NewMsg -> gid = to_gid;
    user_date_t Srvdate = DateNow();
    user_time_t Srvtime = TimeNow();
    char Srvdatetime[25];
    sprintf(Srvdatetime ,"%04d-%02d-%02d %02d:%02d:%02d",Srvdate.year ,Srvdate.month ,Srvdate.day,Srvtime.hour ,Srvtime.minute ,Srvtime.second);
    strcpy(NewMsg->time ,Srvdatetime);
    NewMsg->from_uid = gl_uid;
    strcpy(NewMsg->msg ,msg);
    NewMsg -> next = NULL;
    group_msg_t *m;
    List_AddTail(group_msg_list ,m ,NewMsg);    
    cJSON *root = JSON_Cre_Obj();
    cJSON *item = JSON_Cre_Str("p");
    JSON_Add_Item_To_Obj(root ,"type" ,item);
    item = JSON_Cre_Num(gl_uid);
    JSON_Add_Item_To_Obj(root ,"from_uid" ,item);
    item = JSON_Cre_Num(to_gid);
    JSON_Add_Item_To_Obj(root ,"to_gid" ,item);
    item = JSON_Cre_Str(msg);
    JSON_Add_Item_To_Obj(root ,"msg" ,item);
    char *out = JSON_prt(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) <= 0)
    {
        printf("服务器失去响应!\n");
        rtn = 0;
    }
    JSON_shanchu(root);
    free(out);
    return rtn;
}
void Chat_Srv_Show_Priva_Rec2(const char *JSON)
{
    private_msg_t *NewMsg = (private_msg_t *)malloc(sizeof(private_msg_t));
    cJSON *root = JSON_Par(JSON);
    cJSON *item = JSON_Get_Obj_Item(root ,"from_uid");
    NewMsg->from_uid = item -> valueint;
    item = JSON_Get_Obj_Item(root ,"msg");
    strcpy(NewMsg->msg , item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"time");
    strcpy(NewMsg->time , item -> valuestring);
    friends_t * f;
    List_For_Each(fri_list ,f)
    {
        if(f->uid == NewMsg->from_uid)
        {
            strcpy(NewMsg->name ,f->name);
            (f->NewMsgNum) ++;
        }
    }
    NewMsg->next =NULL;
    JSON_shanchu(root);
    if(NewMsg -> from_uid == gl_uid)
    {
        printf("\t\e[32m%s\e[0m ",NewMsg->time);
        printf("本人\n");
        printf("\t  \e[1m%s\e[0m\n",NewMsg -> msg);
    }
    else
    {
        printf("\t\e[31m%s\e[0m ",NewMsg->time);
        printf("%s\n",NewMsg -> name);
        printf("\t  \e[1m%s\e[0m\n",NewMsg -> msg);
    }
}
void Chat_Srv_Get_Priva_Rec(int fuid)
{
    cJSON *root = JSON_Cre_Obj();
    cJSON_AddStringToObject(root ,"type" ,"E");
    cJSON_AddNumberToObject(root ,"uid" ,gl_uid);
    cJSON_AddNumberToObject(root ,"fuid" ,fuid);
    char *out = JSON_prt(root);
    JSON_shanchu(root);
    if(send(sockfd ,(void *)out ,MSG_LEN ,0) <= 0)
    {
        printf("服务器失去响应!\n");
    }
    free(out);
    system("clear");
    sleep(1);
}