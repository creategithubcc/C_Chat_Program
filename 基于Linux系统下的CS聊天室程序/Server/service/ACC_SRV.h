#ifndef _ACCOUNT_SER_H
#define _ACCOUNT_SER_H
#include "./FRI_SRV.h"
 typedef struct online
 {
     int uid;
     int sockfd;
     struct online *next;
 } online_t;
int Acc_Srv_Ch_Onl(int uid ,int is_online ,int sockfd);
int ACC_SRV_SEND_ONL(int uid, int is_online);
int Acc_Srv_Out(int sockfd , char *cJSON);
int Acc_Srv_sig(int sockfd , char * cJSON);
int Acc_Srv_Log(int sockfd ,char * cJSON);
#endif