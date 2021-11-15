#include<stdio.h>
#include<string.h>
#include "./GROUP_UI.h"
#include "../src/Common.h"
#include "../src/List.h"
#include "../service/GROUP_SRV.h"
#include "../service/FRI_SRV.h"
extern group_t *group_list;
extern friends_t *fri_list;
void Gro_UI_Cre()
{
    char gname[30];
    printf("输入创建群名称:");
    sgets(gname,30);
    Gro_Srv_Cre(gname);
}
void Group_UI_ShowList()
{
    static int g_num = 0;
    printf("$$ 群聊(%d)\n" ,g_num);
    g_num = 0;
    group_t * g;
    List_For_Each(group_list ,g)
    {
        printf("   %s",g -> name);
        if(g -> NewMsgNum > 0)
        {
            printf(" (\e[31m%d\e[0m)",g -> NewMsgNum);
        } 
        printf("\n");
        g_num +=1;
    }
}

void Gro_UI_Add_Mem(int gid)
{
    char name[30]; 
    printf("输入邀请好友名:");
    scanf("%30s",name);
    cc_Clear();
    friends_t *f;
    List_For_Each(fri_list ,f)
    {
        if(strcmp(f -> name ,name) == 0) break;
    }
    if(f == NULL)
    {
        printf("%s 不是你好友，没法邀请" ,name);
        getchar();
        return;
    }
    Gro_Srv_Add_Mem(gid ,f -> uid);
}