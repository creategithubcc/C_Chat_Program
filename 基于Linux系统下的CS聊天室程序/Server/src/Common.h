#ifndef _COMMON_H
#define _COMMON_H
#include <time.h>
#include<stdio.h>
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
char *sgets(char *str,int len);
void cc_Clear();
int DateCmp(user_date_t dt1, user_date_t dt2);
user_date_t DateNow();
user_time_t TimeNow();
#endif