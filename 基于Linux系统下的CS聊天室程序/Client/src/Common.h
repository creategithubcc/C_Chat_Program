#ifndef _COMMON_H
#define _COMMON_H
#include <encode.h>
#include <decode.h>
#include<stdio.h>
#include <time.h>
#include<fcntl.h>
typedef struct 
{
	int year;
	int month;
	int day;
}user_date_t;
typedef struct 
{
	int hour;
	int minute;
	int second;
}user_time_t;
void cc_Lock();
void cc_Unlock();
char *sgets(char *str,int len);
void cc_Clear();
int DateCmp(user_date_t dt1, user_date_t dt2);
user_date_t DateNow();
user_time_t TimeNow();
#endif