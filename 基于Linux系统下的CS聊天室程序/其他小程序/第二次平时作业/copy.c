#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<dirent.h>
#include<utime.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/time.h>

void fuzhi_wenjian(char *yuanwenjian,char *mubiaowenjian) //复制文件
{
    int yuan=open(yuanwenjian,0); //打开源文件
    struct stat huanchongqu2;//一个新的缓冲区来记录信息 
    stat(yuanwenjian,&huanchongqu2);//返回关于文件的信息 
    int mubiao=creat(mubiaowenjian,huanchongqu2.st_mode);//创建文件，如果这个文件已经存在的话，就会将长度变为零
    int a;//临时变量
    char huanchongqu[1000]; //开始复制文件
    while((a=read(yuan,huanchongqu,1000))>0)//有文件的话就一直循环 
    {
        if(write(mubiao,huanchongqu,a)!=a) //如果没有写入完全的话 
        {
            printf("创建失败啦!\n");
            exit(0);
        }
    }
    struct utimbuf shijian_huanchongqu;//这一块就是修改时间属性保持一致
    shijian_huanchongqu.actime=huanchongqu2.st_atime;//分别把时间复制过去 
    shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
    utime(mubiaowenjian,&shijian_huanchongqu);
    close(mubiao);// 关闭目标文件句柄 
    close(yuan); //关闭源文件
}

void fuzhi_ruanlianjie(char *yuanwenjian,char *mubiaowenjian) //复制软链接，该文件存放的内容是另一文件的路径名的指向
{
    char huanchongqu[2000],jiu_lianjie[1000];//缓冲区和旧路径 
    getcwd(jiu_lianjie,sizeof(jiu_lianjie));//getcwd()会将当前工作目录的绝对路径复制到所指的内存空间中
    strcat(jiu_lianjie,"/");//拼接到当前目录 
    memset(huanchongqu,0,sizeof(huanchongqu));//清空缓冲区 
    readlink(yuanwenjian,huanchongqu,2000);//找到软链接的目标
    symlink(strcat(jiu_lianjie,yuanwenjian),mubiaowenjian);//创建新的软链接
    struct stat huanchongqu2;//用来存放文件的信息（时间） 
    lstat(yuanwenjian,&huanchongqu2);//设置时间属性一致，获取文件属性 
    struct timeval shijian_huanchongqu[2]; //在这里，utimes是需要使用timeval数组来修改时间的 
    shijian_huanchongqu[0].tv_sec=huanchongqu2.st_atime;
    shijian_huanchongqu[0].tv_usec=0;
    shijian_huanchongqu[1].tv_sec=huanchongqu2.st_mtime;
    shijian_huanchongqu[1].tv_usec=0;
    lutimes(mubiaowenjian,shijian_huanchongqu);//时间戳 修改 
}

void fuzhi_mulu(char *yuanwenjian,char *mubiaowenjian) //复制目录
{
    DIR *wenjian_liebiao;
    struct stat huanchongqu2;
    char yuan[1000],mubiao[1000];//创建两个数组分别代表源文件与目标文件的数组 
    strcpy(mubiao,mubiaowenjian);//把mubiaowenjian的数据拷贝到mubiao 
    strcat(mubiao,"/");//接到当前文件夹 
    if((wenjian_liebiao=opendir(yuanwenjian))==NULL)
    {
        printf("opendir errorII!\n");
        exit(0);
    }
    struct dirent *chucun;//存储文件夹的文件信息
    while((chucun=readdir(wenjian_liebiao))!=NULL)//开始遍历所有文件
    {
        if(chucun->d_type==10)//如果是软链接的话 
        {
            memset(yuan,0,sizeof(yuan));//清空 
            strcpy(yuan,yuanwenjian);//拷贝 
            strcat(yuan,"/");//拼接 
            strcat(yuan,chucun->d_name);
            strcat(mubiao,chucun->d_name);
            fuzhi_ruanlianjie(yuan,mubiao);
            strcpy(mubiao,mubiaowenjian);
            strcat(mubiao,"/");
        }
        else if(chucun->d_type==4)//如果遍历到的是文件夹的话 
        {
            if(strcmp(chucun->d_name,"..")!=0&&strcmp(chucun->d_name,".")!=0)//如果不是父目录也不是当前目录的话
            { 
                memset(yuan,0,sizeof(yuan));//清空yuan的数据 
                strcpy(yuan,yuanwenjian);//拷贝 
                strcat(yuan,"/");//拼接到当前文件夹 
                strcat(yuan,chucun->d_name);
                strcat(mubiao,chucun->d_name);
                stat(yuan,&huanchongqu2);//返回信息 
                mkdir(mubiao,huanchongqu2.st_mode);//创建一个新的目录 
                fuzhi_mulu(yuan,mubiao);//递归,进入子目录继续拷贝
                struct utimbuf shijian_huanchongqu;//修改时间的一致性 
                shijian_huanchongqu.actime=huanchongqu2.st_atime;
                shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
                utime(mubiao,&shijian_huanchongqu);
                strcpy(mubiao,mubiaowenjian);
                strcat(mubiao,"/");
            }
        }
        else//如果是文件的话 
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
    closedir(wenjian_liebiao);//关闭参数dir所指的目录
}

int main(int argc, char *argv[])
{
    DIR *wenjian_liebiao;
    struct stat huanchongqu2;//存储文件属性
    if((wenjian_liebiao=opendir(argv[2]))==NULL) //如果目录下没有该文件夹，就创建这个文件夹 
    {
        stat(argv[1],&huanchongqu2);
        mkdir(argv[2],huanchongqu2.st_mode);
        printf("成功创建文件夹!\n");
    }
    closedir(wenjian_liebiao);//关闭参数dir所指的目录
    fuzhi_mulu(argv[1],argv[2]);
    struct utimbuf shijian_huanchongqu;//修改时间
    shijian_huanchongqu.actime=huanchongqu2.st_atime;
    shijian_huanchongqu.modtime=huanchongqu2.st_mtime;
    utime(argv[2],&shijian_huanchongqu);
    printf("复制完成!\n");
    return 0;
}
