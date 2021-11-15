#include <stdio.h>
#include "FRI_UI.h"
#include "./ACC_UI.h"
#include "../src/List.h"
#include "../service/FRI_SRV.h"
extern friends_t *fri_list;
extern int Pri_msg_num;
extern int gl_uid;
int f_num = 0 ,online_num = 0 ,a_num = 0;
void Fri_UI_Show_List()
{
    friends_t *cur_Pos = NULL;
    printf("$$ 我的好友(%d/%d)\n",online_num ,f_num);
    f_num = online_num = 0;
    char *is_online[2] = {"@","\e[32m@\e[0m"};
    char *is_vip[2] = {"","\e[31m"};
    char *sex[2] = {"\e[35m女\e[0m","\e[36m男\e[0m"};
    char *is_follow[2] = {"","\e[31m！\e[0m"};
    char is_msg[2][20]={"",""};
    List_For_Each(fri_list ,cur_Pos)
    {
        if(cur_Pos -> state != 1) 
        {
            a_num +=1;
            continue;
        }
        f_num +=1;
        if(cur_Pos->is_online)
        {
            online_num +=1;
        } 
        sprintf(is_msg[0],"(\e[31m%d\e[0m)" ,cur_Pos -> NewMsgNum);
        printf("   %s %s%s\e[0m %s %s %s\n" ,is_online[cur_Pos->is_online] ,is_vip[cur_Pos->is_vip],cur_Pos -> name ,sex[cur_Pos->sex] ,is_follow[cur_Pos->is_follow],is_msg[(cur_Pos->NewMsgNum == 0)]);
    }
}
void Friends_UI_ShowApply()
{
    printf("$$ 申请列表(%d)\n" ,a_num);
    a_num = 0;
    friends_t *cur_Pos;
    List_For_Each(fri_list ,cur_Pos)
    {
        if(cur_Pos -> state != 0 || cur_Pos -> uid == gl_uid)
        {
            continue;
        } 
        printf("   %s 申请加你为好友\n",cur_Pos->name);
    }
}
void Friends_UI_Add()
{
    char fname[30];
    printf("请输入添加好友名:");
    scanf("%30s",fname);
    cc_Clear();
    friends_t *f;
    List_For_Each(fri_list ,f)
    {
        if(strcmp(f->name ,fname) == 0) 
        {
            printf("%s 已经成为你的好友了!",fname);
            getchar();
            return;
        }
    }
    Fri_Srv_Send_Add(fname);
}

void Friends_UI_Apply()
{
    friends_t *f;
    char choice;
    List_For_Each(fri_list ,f)
    {
        if(f -> state != 0)
        {
            continue;
        } 
        printf("是否同意 %s 的好友请求?\n请输入:(Y/N)", f->name);
        scanf("%c",&choice);
        if(choice == 'y')
        {
            Fri_Srv_App(f -> uid ,gl_uid ,1);
            f -> state = 1;
        }
        else if(choice == 'n')
        {
            Fri_Srv_App(f->uid ,gl_uid ,0);
            List_FreeNode(fri_list ,f ,friends_t);
        }
    }
}