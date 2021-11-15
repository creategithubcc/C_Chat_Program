#ifndef _ACCOUNT_SRV_H
#define _ACCOUNT_SRV_H
#include "CONNECT.h"
#include "../src/JSON.h"
void Acc_Srv_Rec_Onl(char *cJSON);
int Acc_Srv_Out(int uid);
int Acc_Srv_sig(const char *name , int sex ,const char *password);
int Acc_Srv_Log(const char *name , const char *password);
#endif