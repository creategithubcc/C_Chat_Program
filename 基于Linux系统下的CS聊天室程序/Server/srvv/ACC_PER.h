#ifndef _ACCOUNT_PERSIST_H
#define _ACCOUNT_PERSIST_H
int Acc_Per_Ch_Is_Onl(int uid ,int is_online);
int Acct_Per_Is_user_Name(const char * name);
int Acc_Per_Add_User(const char * name ,int sex, const char *password);
int Acc_Per_Mat_User_And_Password(int uid , const char * password);
char * Acc_Per_Get_User_Name_From_Uid(int uid);
#endif