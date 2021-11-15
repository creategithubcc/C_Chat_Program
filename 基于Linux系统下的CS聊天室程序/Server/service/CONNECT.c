#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <unistd.h>
#include "../src/JSON.h"
#include "./ACC_SRV.h"
#include "./FRI_SRV.h"
#include "./CONNECT.h"
#include "./CHAT_SRV.h"
#include "./GROUP_SRV.h"
#include "../srvv/FRI_PER.h"
#include "../src/List.h"
#define LISTEN_NUM 12 
#define MSG_LEN 1024
online_t *Onl_List;
void * thread(void *arg)
{
    int ret ,recv_len;
    char buf[MSG_LEN];
    cJSON *root ,*item;
    char choice[3];
    int client_fd = (int)(long)arg;
    while(1)
    {
        recv_len = 0;
        while(recv_len < MSG_LEN)
        {
            ret = 0;
            if((ret = recv(client_fd , buf + recv_len , MSG_LEN - recv_len , 0)) <= 0)
            {
                int uid = Acc_Srv_Ch_Onl(-1 , 0 ,client_fd);
                if(uid != -1)
                {
                    ACC_SRV_SEND_ONL(uid ,0);
                }
                perror("recv");
                return NULL;
            }
            recv_len += ret;
        }
        root = JSON_Par(buf);
        item = JSON_Get_Obj_Item(root,"type");
        strcpy(choice ,item -> valuestring);
        JSON_shanchu(root);
        switch(choice[0])
        {
            case 'L' :
                Acc_Srv_Log(client_fd , buf);
                break;
            case 'S' :
                Acc_Srv_sig(client_fd , buf);
                break;
            case 'A' :
                Fri_Srv_Add(client_fd, buf);
                break;
            case 'G' :
                Fri_Srv_Get_List(client_fd ,buf);
                break;
            case 'g' :
                Gro_Srv_Get_List(client_fd ,buf);
                break;
            case 'P' :
                Chat_Srv_Priva2(client_fd,buf);
                break;
            case 'p':
                Chat_Srv_Group(client_fd ,buf);
                break;
            case 'F' :
                Chat_Srv_File(buf);
                break;
            case 'O' :
                Acc_Srv_Out(client_fd ,buf);
                break;
            case 'a':
                Fri_Srv_App(client_fd ,buf);
                break;
            case 'c':
                Gro_Srv_Cre(client_fd ,buf);
                break;
            case 'M' :
                Gro_Srv_Add_Mem(client_fd ,buf);
                break;
            case 'm':
                Gro_Srv_Show_Mem(client_fd ,buf);
                break;
            case 'Q' :
                Gro_Srv_Quit(client_fd ,buf);
                break;
            case 'E' :
                Chat_Srv_Send_Priva_Res(client_fd ,buf);
                break;
        }
    }
    return NULL;
}
void Connect(int port)
{
    int sockfd;
    int client_fd;
    int optval;
    int len;
    List_Init(Onl_List , online_t);
    struct sockaddr_in serv_addr , client_addr;
    len = sizeof(struct sockaddr_in);
    memset(&serv_addr , 0 ,len);
    memset(&client_addr , 0 , len);
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockfd = socket(AF_INET , SOCK_STREAM , 0);
    if(sockfd < 0) 
    {
        perror("socket");
        exit(0);
    }
    optval = 1;
    if(setsockopt(sockfd , SOL_SOCKET , SO_REUSEADDR , (void *)&optval , sizeof(int)) < 0)
    {
        perror("socksetopt");
        exit(0);
    }
    if(bind(sockfd , (struct sockaddr *)&serv_addr , len) < 0)
    {
        perror("bind");
        exit(0);
    }
    if(listen(sockfd , LISTEN_NUM) < 0)
    {
        perror("listen");
        exit(0);
    }
    while(1)
    {
        client_fd = accept(sockfd , (struct sockaddr *)&client_addr , (socklen_t *)&len);
        if(client_fd < 0) 
        {
            perror("accept");
            exit(0);
        }
        pthread_t thid;
        pthread_create(&thid , NULL , thread ,(void *)(long)client_fd);
    }
}