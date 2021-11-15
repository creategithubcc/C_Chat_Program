#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"./Common.h"
int mutex_srv = 0;
void cc_Unlock()
{
    mutex_srv = 0;
}
void cc_Lock()
{
    while(mutex_srv == 0);
}
void cc_Clear()
{
	char ch;
	while((ch=getchar())!='\n' && ch!=EOF){}
}
char *sgets(char *str,int len)
{
	fgets(str,len,stdin);
	if(str[strlen(str)-1]=='\n')
	{
		str[strlen(str)-1]='\0';
	}
	return str;
}
int DateCmp(user_date_t dt1, user_date_t dt2) 
{
	if (dt1.year < dt2.year)
	{
		return -1;
	}
	else if (dt1.month < dt2.month && dt1.year == dt2.year)
	{
		return -1;
	}
	else if (dt1.month == dt2.month && dt1.day < dt2.day && dt1.year == dt2.year)
	{
		return -1;
	}	
	else if (dt1.day == dt2.day && dt1.year == dt2.year && dt1.month == dt2.month)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}
user_date_t DateNow() 
{
	user_date_t curDate;
	time_t now; 
	struct tm *timeNow; 
	time(&now);
	timeNow = localtime(&now);
	curDate.year=timeNow->tm_year+1900;
	curDate.month=timeNow->tm_mon+1;
	curDate.day=timeNow->tm_mday;
	return curDate;
}
user_time_t TimeNow()
{
	user_time_t curTime;
	time_t now;   
	struct tm *timeNow;
	time(&now);
	timeNow = localtime(&now);
	curTime.hour=timeNow->tm_hour;
	curTime.minute=timeNow->tm_min;
	curTime.second=timeNow->tm_sec;
	return curTime;
}