#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
int strl,flag=0,cot=0,i,n=0;
char cmd[1000];
char tmp[1000];
char his[100000];
char ans; 
char * p;

int main(int argc, char ** argv)
{
  while(1) 
  {
    memset(cmd,0,1000);
    printf("输入shell命令:");
    //scanf("%s",&cmd);
	fgets(cmd,1000,stdin);//最开始用scanf写，但总是错，后来改成这个就可以了 
	cmd[strlen(cmd)-1]=0;
	if(!strncasecmp(cmd,"cd",2))//strcasecmp忽略大小写比较字符串
	{
		ans=1;
	} 
	else if(!strncasecmp(cmd,"ls",2))
	{
		ans=2;
	} 
	else if(!strncasecmp(cmd,"pwd",3))
	{
		ans=3;
	} 
	else if(!strncasecmp(cmd,"rm",2))
	{
		ans=4;
	} 
	else if(!strncasecmp(cmd,"mkdir",5))
	{
		ans=5;
	} 
	else if(!strncasecmp(cmd,"mv",2))
	{
		ans=6;
	} 
	else if(!strncasecmp(cmd,"cp",2))
	{
		ans=7;
	} 
	else if(!strncasecmp(cmd,"exit",4))
	{
		ans=8;
	} 
	else if(!strncasecmp(cmd,"history",7))
	{
		ans=9;
	} 
	else
	{
		ans=0;
	}



    switch(ans)
	{
	    case 0://else
	    	printf("输入错误！请重试！！\n");flag=0;
	    	break;
		  		
	    case 1://cd
			if(chdir(cmd+3)!=0) 
			{
				printf("切换(%s) 错误!%s\n",cmd+3,strerror(errno));
			}
			printf("正在切换:'%s' \n",getcwd(tmp,100));
			flag=1;
			break;
			
	    case 2://ls
	    	system(cmd);flag=1;
		    break;
  		
  		case 3://pwd
		    system(cmd);flag=1;
		    break;
		    
		case 4://rm
		    system(cmd);flag=1;
		    break;    
		
		case 5://mkdir
		    system(cmd);flag=1;
		    break;
		
		case 6://mv
			system(cmd);flag=1;
		    break;
					    
	    case 7://cp
			system(cmd);flag=1;
			break;
	    
	    case 8://exit
		    printf("退出shell进程！\n");flag=1;
			return 0;
 
		case 9://history
			printf("总共有%d条历史命令！\n",cot);
			for(int j=0;j<strl+n-1;j++)
			{
				if(his[j]=='?')
				{
					printf("\n"); 
					continue;
				}
				printf("%c",his[j]);
			}
			printf("\n");
			flag=1;
			break;
	      
    }
    
    if(flag==1)
    {
    	strl=strlen(cmd);//每次一运行一个命令flag=1，然后在这里记录一下输入的字符串 
	    for(i=n;i<strl+n;i++)
	    {
	    	his[i]=cmd[i-n];
		}
		his[i+1]='?';//扫描完后加个‘？’之后每次一扫到问号就自动回车到下一段 
		n=i+2;
		flag=0;
		cot++;
		printf("ok!\n"); 
	}
	  
  }
  
}

