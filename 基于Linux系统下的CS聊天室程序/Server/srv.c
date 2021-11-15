#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include "src/JSON.h"
#include "srvv/MySQL.h"
#include "service/CONNECT.h"
int main()
{
    char buf[1000],host[50] ,user[30],pass[50],database[50];
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
    item = JSON_Get_Obj_Item(root ,"user");
    strcpy(user ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"pass");
    strcpy(pass ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"database");
    strcpy(database ,item -> valuestring);
    item = JSON_Get_Obj_Item(root ,"port");
    int port = item -> valueint;
    close(fd);
    JSON_shanchu(root);
    if(MYSQL_CONNECT(host ,user ,pass ,database) == 0)
    {
        printf("数据库连接失败!是不是没开数据库呢！\n");
        exit(0);
    }
    Connect(port);
}