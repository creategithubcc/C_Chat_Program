#include<stdio.h>
#include "./MAIN_UI.h"
#include "./ACC_UI.h"
#include "./FRI_UI.h"
#include "./CHAT_UI.h"
#include "./GROUP_UI.h"
#include "../service/ACC_SRV.h"
#include "../service/FRI_SRV.h"
#include "../service/CHAT_SRV.h"
#include "../service/GROUP_SRV.h"
extern int gl_uid;
void Main_UI_Hello()
{
    int choice;
    do{
        if(gl_uid > 0)
        {
            Main_UI_Menu();
        }
        system("clear");
        printf("\t1.登录\n""\t2.注册\n""\t3.退出\n""命令行:");
        scanf("%d" , &choice);
        cc_Clear();
        switch(choice)
        {
            case 1:
                gl_uid = ACC_UI_LOG();
                break;
            case 2:
                ACC_UI_SIG();
                break;
            case 3:
                return;
                break;
            default:
                break;
        }
    }while(1);
}
void Main_UI_Menu()
{
    Chat_Srv_Init_List();
    Fri_Srv_Get_List();
    Gro_Srv_Get_List();
    char choice;
    do{
        system("clear");
        Fri_UI_Show_List();
        Group_UI_ShowList();
        Friends_UI_ShowApply();
        printf( "###############################\n");
        printf( "1.选择好友|2.选择群聊|3.申请批准\n"
                "4.好友添加|5.群聊创建|6.注销登录\n");
        printf( "###############################\n" "命令行:");
        scanf("%c",&choice);
        if(choice == '\n')
        {
            continue;
        } 
        cc_Clear();
        switch(choice)
        {
            case '1':
                Chat_UI_Priva();
                break;
            case '2':
                Chat_UI_Group();
                break;
            case '3':
                Friends_UI_Apply();
                break;
            case '4':
                Friends_UI_Add();
                break;
            case '5':
                Gro_UI_Cre();
                break;
            case '6':
                Acc_Srv_Out(gl_uid);
                break;
        }
    }while(choice != '6');
}