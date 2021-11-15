#include <stdio.h>
#include "./ACC_UI.h"
#include "../service/ACC_SRV.h"
#include "../src/Common.h"
int ACC_UI_SIG()
{
    int sex;
    char name[30] , password[30];
    printf("请输入你准备注册的用户名:");
    scanf("%30s",name);
    cc_Clear();
    while(1)
    {
        printf("请输入性别(男/女):");
        scanf("%30s",password);
        cc_Clear();
        if(strcmp(password ,"男") == 0)
        {
            sex = 1;
            break;
        }
        else if(strcmp(password ,"女") == 0)
        {
            sex = 0;
            break;
        }
        else
        {
            printf("啊这...难道您就是传说中的不男不女？！\n");
        }
    }
    printf("输入你的密码:");
    scanf("%s",password);
    cc_Clear();
    return Acc_Srv_sig(name ,sex ,password);
}
int ACC_UI_LOG()
{
    char name[30] , password[30];
    printf("请输入你的用户名:");
    scanf("%s",name);
    cc_Clear();
    printf("请输入你的密码:");
    scanf("%s",password);
    cc_Clear();
    return Acc_Srv_Log(name , password);
}