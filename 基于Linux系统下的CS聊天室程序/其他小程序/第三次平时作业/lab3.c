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
    printf("����shell����:");
    //scanf("%s",&cmd);
	fgets(cmd,1000,stdin);//�ʼ��scanfд�������Ǵ������ĳ�����Ϳ����� 
	cmd[strlen(cmd)-1]=0;
	if(!strncasecmp(cmd,"cd",2))//strcasecmp���Դ�Сд�Ƚ��ַ���
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
	    	printf("������������ԣ���\n");flag=0;
	    	break;
		  		
	    case 1://cd
			if(chdir(cmd+3)!=0) 
			{
				printf("�л�(%s) ����!%s\n",cmd+3,strerror(errno));
			}
			printf("�����л�:'%s' \n",getcwd(tmp,100));
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
		    printf("�˳�shell���̣�\n");flag=1;
			return 0;
 
		case 9://history
			printf("�ܹ���%d����ʷ���\n",cot);
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
    	strl=strlen(cmd);//ÿ��һ����һ������flag=1��Ȼ���������¼һ��������ַ��� 
	    for(i=n;i<strl+n;i++)
	    {
	    	his[i]=cmd[i-n];
		}
		his[i+1]='?';//ɨ�����Ӹ�������֮��ÿ��һɨ���ʺž��Զ��س�����һ�� 
		n=i+2;
		flag=0;
		cot++;
		printf("ok!\n"); 
	}
	  
  }
  
}

