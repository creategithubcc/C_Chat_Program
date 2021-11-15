#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<utime.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/time.h>

void fuzhi_wenjian(char *yuanwenjian,char *mubiaowenjian) //�����ļ�
{
    int yuan=open(yuanwenjian,0); //��Դ�ļ�
    struct stat huanchongqu2;//һ���µĻ���������¼��Ϣ 
    stat(yuanwenjian,&huanchongqu2);//���ع����ļ�����Ϣ 
    int mubiao=creat(mubiaowenjian,huanchongqu2.st_mode);//�����ļ����������ļ��Ѿ����ڵĻ����ͻὫ���ȱ�Ϊ��
    int a;//��ʱ����
    char huanchongqu[1000]; //��ʼ�����ļ�
    while((a=read(yuan,huanchongqu,1000))>0)//���ļ��Ļ���һֱѭ�� 
    {
        if(write(mubiao,huanchongqu,a)!=a) //���û��д����ȫ�Ļ� 
        {
            printf("����ʧ����!\n");
            exit(0);
        }
    }
    struct utimbuf shijian_huanchongqu;//��һ������޸�ʱ�����Ա���һ��
    shijian_huanchongqu.actime=huanchongqu2.st_atime;//�ֱ��ʱ�临�ƹ�ȥ 
    shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
    utime(mubiaowenjian,&shijian_huanchongqu);
    close(mubiao);// �ر�Ŀ���ļ���� 
    close(yuan); //�ر�Դ�ļ�
}

void fuzhi_ruanlianjie(char *yuanwenjian,char *mubiaowenjian) //���������ӣ����ļ���ŵ���������һ�ļ���·������ָ��
{
    char huanchongqu[2000],jiu_lianjie[1000];//�������;�·�� 
    getcwd(jiu_lianjie,sizeof(jiu_lianjie));//getcwd()�Ὣ��ǰ����Ŀ¼�ľ���·�����Ƶ���ָ���ڴ�ռ���
    strcat(jiu_lianjie,"/");//ƴ�ӵ���ǰĿ¼ 
    memset(huanchongqu,0,sizeof(huanchongqu));//��ջ����� 
    readlink(yuanwenjian,huanchongqu,2000);//�ҵ������ӵ�Ŀ��
    symlink(strcat(jiu_lianjie,yuanwenjian),mubiaowenjian);//�����µ�������
    struct stat huanchongqu2;//��������ļ�����Ϣ��ʱ�䣩 
    lstat(yuanwenjian,&huanchongqu2);//����ʱ������һ�£���ȡ�ļ����� 
    struct timeval shijian_huanchongqu[2]; //�����utimes����Ҫʹ��timeval�������޸�ʱ��� 
    shijian_huanchongqu[0].tv_sec=huanchongqu2.st_atime;
    shijian_huanchongqu[0].tv_usec=0;
    shijian_huanchongqu[1].tv_sec=huanchongqu2.st_mtime;
    shijian_huanchongqu[1].tv_usec=0;
    lutimes(mubiaowenjian,shijian_huanchongqu);//ʱ��� �޸� 
}

void fuzhi_mulu(char *yuanwenjian,char *mubiaowenjian) //����Ŀ¼
{
    DIR *wenjian_liebiao;
    struct stat huanchongqu2;
    char yuan[1000],mubiao[1000];//������������ֱ����Դ�ļ���Ŀ���ļ������� 
    strcpy(mubiao,mubiaowenjian);//��mubiaowenjian�����ݿ�����mubiao 
    strcat(mubiao,"/");//�ӵ���ǰ�ļ��� 
    if((wenjian_liebiao=opendir(yuanwenjian))==NULL)
    {
        printf("opendir errorII!\n");
        exit(0);
    }
    struct dirent *chucun;//�洢�ļ��е��ļ���Ϣ
    while((chucun=readdir(wenjian_liebiao))!=NULL)//��ʼ���������ļ�
    {
        if(chucun->d_type==10)//����������ӵĻ� 
        {
            memset(yuan,0,sizeof(yuan));//��� 
            strcpy(yuan,yuanwenjian);//���� 
            strcat(yuan,"/");//ƴ�� 
            strcat(yuan,chucun->d_name);
            strcat(mubiao,chucun->d_name);
            fuzhi_ruanlianjie(yuan,mubiao);
            strcpy(mubiao,mubiaowenjian);
            strcat(mubiao,"/");
        }
        else if(chucun->d_type==4)//��������������ļ��еĻ� 
        {
            if(strcmp(chucun->d_name,"..")!=0&&strcmp(chucun->d_name,".")!=0)//������Ǹ�Ŀ¼Ҳ���ǵ�ǰĿ¼�Ļ�
            { 
                memset(yuan,0,sizeof(yuan));//���yuan������ 
                strcpy(yuan,yuanwenjian);//���� 
                strcat(yuan,"/");//ƴ�ӵ���ǰ�ļ��� 
                strcat(yuan,chucun->d_name);
                strcat(mubiao,chucun->d_name);
                stat(yuan,&huanchongqu2);//������Ϣ 
                mkdir(mubiao,huanchongqu2.st_mode);//����һ���µ�Ŀ¼ 
                fuzhi_mulu(yuan,mubiao);//�ݹ�,������Ŀ¼��������
                struct utimbuf shijian_huanchongqu;//�޸�ʱ���һ���� 
                shijian_huanchongqu.actime=huanchongqu2.st_atime;
                shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
                utime(mubiao,&shijian_huanchongqu);
                strcpy(mubiao,mubiaowenjian);
                strcat(mubiao,"/");
            }
        }
        else//������ļ��Ļ� 
        {
            memset(yuan,0,sizeof(yuan));
            strcpy(yuan,yuanwenjian);
            strcat(yuan,"/");
            strcat(yuan,chucun->d_name);
            strcat(mubiao,chucun->d_name);
            fuzhi_wenjian(yuan,mubiao);
            strcpy(mubiao,mubiaowenjian);
            strcat(mubiao,"/");
        }
    }
    closedir(wenjian_liebiao);//�رղ���dir��ָ��Ŀ¼
}

int main(int argc, char *argv[])
{
    DIR *wenjian_liebiao;
    struct stat huanchongqu2;//�洢�ļ�����
    if((wenjian_liebiao=opendir(argv[2]))==NULL) //���Ŀ¼��û�и��ļ��У��ʹ�������ļ��� 
    {
        stat(argv[1],&huanchongqu2);
        mkdir(argv[2],huanchongqu2.st_mode);
        printf("�ɹ������ļ���!\n");
    }
    closedir(wenjian_liebiao);//�رղ���dir��ָ��Ŀ¼
    fuzhi_mulu(argv[1],argv[2]);
    struct utimbuf shijian_huanchongqu;//�޸�ʱ��
    shijian_huanchongqu.actime=huanchongqu2.st_atime;
    shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
    utime(argv[2],&shijian_huanchongqu);
    printf("�������!\n");
    return 0;
}
