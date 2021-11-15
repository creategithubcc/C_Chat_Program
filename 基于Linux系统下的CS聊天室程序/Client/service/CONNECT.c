#include "./GROUP_SRV.h"

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <string.h>
#include "./ACC_SRV.h"
#include "./CHAT_SRV.h"
#include "./CONNECT.h"
#include "./FRI_SRV.h"
#include "../src/JSON.h"
#define MSG_LEN 1024
int sockfd;
extern int mutex_srv;
char massage[MSG_LEN];
void * thread(void *arg)
{
    int ret ,recv_len;
    cJSON *root ,*item;
    if(arg == NULL)
    {
        arg = NULL;
    } 
    while(1)
    {
        if(mutex_srv)
        {
            continue;
        } 
        recv_len = 0;
        while(recv_len < MSG_LEN)
        {
            ret = 0;
            if((ret = recv(sockfd ,massage + recv_len ,MSG_LEN - recv_len,0)) <= 0)
            {
                perror("接受: 服务器失去响应！");
                exit(0);
            }
            recv_len += ret;
        }
        root = JSON_Par(massage);
        item = JSON_Get_Obj_Item(root ,"type");
        switch(item -> valuestring[0])
        {
            case 'A' :
                Friends_Srv_Recv_Add(massage);
                break;
            case 'a' :
                Friends_Srv_App_Res(massage);
                break;
            case 'P' :
                Chat_Srv_Rec_Priva(massage);
                break;
            case 'p':
                Chat_Srv_Rec_Group(massage);
                break;
            case 'F' :
                Chat_Srv_Rec_File(massage);
                break;
            case 'Q' :
                break;
            case 'R' :
                mutex_srv = 1;
                break;
            case 'L' :
                mutex_srv = 1;
                break;
            case 'l' :
                mutex_srv = 1;
                break;
            case 'I' :
                Acc_Srv_Rec_Onl(massage);
                break;
            case 'J' :
                Group_Srv_Join(massage);
                break;
            case 'm' :
                Gro_Srv_Show_Mem(massage);
                break;
            case 'D' :
                Group_Srv_Del(massage);
                break;
            case 'E' :
                Chat_Srv_Show_Priva_Rec2(massage);
                break;
        }
    }
    return NULL;
}
void Connect(const char *host ,int port)
{
    struct sockaddr_in clie_addr;
    pthread_t thid;
    memset(&clie_addr , 0 ,sizeof(struct sockaddr_in));
    clie_addr.sin_family = AF_INET;
    clie_addr.sin_port = htons(port);
    clie_addr.sin_addr.s_addr = inet_addr(host);
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if(sockfd < 0)
    {
        perror("socket");
        exit(0);
    }
    if(connect(sockfd , (struct sockaddr *)&clie_addr,sizeof(struct sockaddr_in)) < 0)
    {
        perror("connect");
        exit(0);
    }
    pthread_create(&thid ,NULL ,thread ,NULL);
}