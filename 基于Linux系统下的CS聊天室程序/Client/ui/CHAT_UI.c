#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include"./GROUP_UI.h"
#include"../src/Common.h"
#include"../service/CHAT_SRV.h"
#include"../service/FRI_SRV.h"
#include"../service/GROUP_SRV.h"
#include"../src/List.h"
extern friends_t * fri_list;
extern group_t * group_list;
extern int gl_uid;
extern private_msg_t* Pri_msg_list;
extern group_msg_t* group_msg_list;
group_t *curGroup = NULL;
void Chat_UI_Priva()
{
    friends_t *cur_Fri = NULL;
    char fname[30];
    char msg[1000];
    friends_t * f;
    while(1)
    {
        printf("请输入你的好友用户名:");
        scanf("%30s",fname);
        cc_Clear();
        List_For_Each(fri_list ,f)
        {
            if(strcmp(f->name ,fname) == 0)
            {
                cur_Fri = f;
            }
        }
        if(cur_Fri == NULL )
        {
            printf("%s ta不是你的好友." , fname);
            getchar();
            return;
        }else
        {
            break;
        }
    }
    int this_msg_num;
    private_msg_t * m;
    do
    {
        system("clear");
        printf( " @                私聊               @\n");
        this_msg_num = 0;
        List_For_Each(Pri_msg_list ,m)
        {
            if(m -> from_uid == gl_uid  || m -> from_uid == cur_Fri -> uid)
            {
                this_msg_num +=1;
            }
        }

        List_For_Each(Pri_msg_list ,m)
        {
            if( m-> from_uid == cur_Fri -> uid )
            {
                if(this_msg_num > 10)
                {
                    List_FreeNode(Pri_msg_list ,m ,private_msg_t);
                    this_msg_num -=1;
                }
                else
                {
                    printf("\t\e[31m%s\e[0m ",m->time);
                    printf("%s\n",m -> name);
                    printf("\t  \e[1m%s\e[0m\n",m -> msg);

                }
            }
            else if(m -> from_uid == gl_uid)
            {
                if(this_msg_num > 10)
                {
                    List_FreeNode(Pri_msg_list ,m ,private_msg_t);
                    this_msg_num -=1;
                }
                else
                {
                    printf("\t\e[32m%s\e[0m ",m->time);
                    printf("本人\n");
                    printf("\t  \e[1m%s\e[0m\n",m -> msg);
                }
            }
        }
        printf( "########################################\n"
                "#     /fanhui  :  返回上一级            #\n"             
                "#     /fasong  :  发送文件              #\n"
                "#     /jilu    :  聊天记录              #\n"
                "#     回车     :  发送/刷新消息         #\n"
                "########################################\n");
        printf("消息/功能:");
        sgets(msg ,1000);
        if(*msg == '\0')
        {
            continue;
        } 
        else if(strcmp(msg,"/fanhui") == 0) 
        {
            cur_Fri -> NewMsgNum = 0;
            return;   
        }
        else if(strcmp(msg,"/fasong") == 0)
        {
            if(cur_Fri -> is_online == 0)
            {
                printf("好友不在线,无法发送！\n");
                getchar();
                continue;
            }
            char filename[100];
            while(1)
            {
                printf("输入你的文件路径:");
                sgets(filename ,100);
                if(*filename == '\0') 
                {
                    break;
                }
                if(Chat_Srv_Send_File(filename ,cur_Fri -> uid))
                {
                   printf("发送成功！");
                }
                getchar();
                break;
            }
        }
        else if(strcmp(msg,"/jilu") == 0)
        {
            Chat_Srv_Get_Priva_Rec(cur_Fri -> uid);
            getchar();
        }
        else
        {
            Chat_Srv_Send_Priva(cur_Fri -> uid,msg);
        }
    }while(1);
}

void Chat_UI_Group()
{
    group_t *g = NULL;
    char msg[1000];
    char gname[30];
    while(1){
        printf("请输群名称:");
        scanf("%s",gname);
        cc_Clear();
        List_For_Each(group_list ,g)
        {
            if(strcmp(g->name ,gname) == 0)
            {
                curGroup = g;
            }
        }
        if(curGroup == NULL )
        {
            printf("你不在群聊 %s " , gname);
            getchar();
            return;
        }
        else
        {
            break;
        }
    }
    int this_msg_num;
    group_msg_t * m;
    do
    {
        if(curGroup == NULL)
        {
            return;
        } 
        system("clear");
        printf( "########################################\n"
                " #                群聊               #\n"
                "########################################\n");
        this_msg_num = 0;
        List_For_Each(group_msg_list ,m)
        {
            if(m -> gid == curGroup -> gid || m -> from_uid == gl_uid)
            {
                this_msg_num +=1;
            }
        }

        List_For_Each(group_msg_list ,m)
        {
            if( m-> gid == curGroup -> gid )
            {
                if(this_msg_num > 10)
                {
                    List_FreeNode(group_msg_list ,m ,group_msg_t);
                    this_msg_num -=1;
                }
                else
                {
                    if(m -> from_uid == gl_uid)
                    {
                        printf("\t\e[32m%s\e[0m ",m->time);
                        printf("本人\n");
                        printf("\t  \e[1m%s\e[0m\n",m -> msg);
                    }
                    else 
                    {
                        printf("\t\e[31m%s\e[0m ",m->time);
                        printf("%s\n",m -> uname);
                        printf("\t  \e[1m%s\e[0m\n",m -> msg);
                    }
                }
            }
        }
        printf( "########################################################\n"
                "/fanhui 返回上一页| /jilu  聊天记录| /chengyuan 查看群成员\n"
                "/yaoqing 邀请好友 | /tui 退/解散群 | 回车       发送/刷新\n"
                "########################################################\n");
        printf("命令:");
        sgets(msg ,1000);
        if(*msg == '\0')
        {
            continue;
        } 
        else if(strcmp(msg,"/fanhui") == 0) 
        {
            curGroup -> NewMsgNum = 0;
            return;   
        }
        else if(strcmp(msg ,"/jilu") == 0)
        {
            //Chat_Srv_Get_Priva_Rec(cur_Fri -> uid);
            //getchar();
        }
        else if(strcmp(msg ,"/chengyuan") == 0)
        {
            system("clear");
            printf("群成员列表(%s):\n", curGroup-> name);
            Group_Srv_Get_Member(curGroup -> gid);
            sleep(1);
            getchar();
        }
        else if(strcmp(msg ,"/yaoqing") == 0)
        {
            Gro_UI_Add_Mem(curGroup -> gid);
        }
        else if(strcmp(msg ,"/tui") == 0)
        {
            Gro_Srv_Quit(curGroup);
            curGroup = NULL;
        }
        else
        {
            Chat_Srv_Send_Group(curGroup -> gid,msg);
        }
    }while(1);
}