#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<fcntl.h>
#include<stdlib.h>
#include "src/JSON.h"
#include "./ui/MAIN_UI.h"
#include "./service/CONNECT.h"
extern int sockfd;
int gl_uid;
int main()
{
    char host[50];
    char buf[1000];
    int fd = open("config.json" ,O_RDONLY);
    if(fd == -1) 
    {
        printf("打开配置文件失败!");
        getchar();
        exit(0);
    }
    read(fd ,buf ,1024);
    cJSON* root = JSON_Par(buf);
    cJSON* item = JSON_Get_Obj_Item(root ,"host");
    strcpy(host ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"port");
    int port = item -> valueint;
    close(fd);
    JSON_shanchu(root);
    Connect(host ,port);
    Main_UI_Hello();
    close(sockfd);
}