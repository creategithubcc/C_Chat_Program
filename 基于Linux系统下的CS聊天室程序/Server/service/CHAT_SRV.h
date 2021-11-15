#ifndef _CHAT_SRV_H
#define _CHAT_SRV_H
void Chat_Srv_File(const char *cJSON);
int Chat_Srv_Get_Fri_Sock(int fuid);
int Chat_Srv_Group(int sockfd ,const char *cJSON);
int Chat_Srv_Priva2(int sockfd ,const char *cJSON);
void Chat_Srv_Send_Priva_Res(int sockfd ,const char *cJSON);
void Chat_Srv_Send_Off_Priva_Msg(int uid);
#endif