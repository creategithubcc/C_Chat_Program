#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>

//控制矩阵尺寸
#define MATRIX_SIZE 5
 //像二维数组一样寻址普通数组  
#define array(arr,i,j)arr[(int) MATRIX_SIZE*(int) i + (int) j]

void fill_matrix(int *matrix);
void print_matrix(int *matrix,int print_width);
int *matrix_page(int *matrix,unsigned long m_size);
void matrix_unmap(int *matrix,unsigned long m_size);
__attribute__ ((noreturn)) void row_multiply(void *row_args);

//矩阵的整数指针
static int *matrix_a,*matrix_b,*matrix_c;

// 线程的参数结构
typedef struct arg_struct
{
  int *a;
  int *b;
  int *c;
  int row;
} thr_args;

//用 1-10的整数填充给定矩阵
void fill_matrix(int *matrix)
{
  for(int i=0;i<MATRIX_SIZE;i++)
  {
    for(int j=0;j<MATRIX_SIZE;j++)
    {
      array(matrix,i,j)=rand()%10+1;
    }
  }
  return;
}

//输出矩阵 
void print_matrix(int *matrix,int print_width)
{
  for(int i=0;i<MATRIX_SIZE;i++)
  {
    for(int j=0;j<MATRIX_SIZE;j++)
    {
      printf("[%*d]",print_width,array(matrix,i,j));
    }
    printf("\n");
  }
  printf("\n");
  return;
}

//使用 mmap() 将给定的矩阵映射到内存页面
int *matrix_page(int *matrix, unsigned long m_size)
{
  matrix=mmap(0,m_size, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
  //如果 mmap()失败就退出
  if(matrix==(void *)-1)
  {
    exit(EXIT_FAILURE);
  }
  memset((void *)matrix,0,m_size);
  return matrix;
}

//从内存页中取消映射给定的矩阵
void matrix_unmap(int *matrix,unsigned long m_size)
{
  //如果 mmap()失败就退出
  if(munmap(matrix, m_size)==-1)
  {
    exit(EXIT_FAILURE);
  }
}

//计算的所有索引
__attribute__ ((noreturn)) void row_multiply(void *row_args)
{
  thr_args *args=(thr_args *) row_args;
  for(int i=0;i<MATRIX_SIZE;i++)
  {
    for(int j=0;j<MATRIX_SIZE;j++)
    {
      int add=array(args->a,args->row,j)*array(args->b,j,i);
      array(args->c,args->row,i)+=add;
    }
  }
  pthread_exit(0);
}

int main(void)
{
  //计算矩阵的内存大小
  unsigned long m_size=sizeof(int) * (unsigned long) (MATRIX_SIZE * MATRIX_SIZE);

  //将 matrix_a、matrix_b 和 matrix_c 映射到内存页
  matrix_a=matrix_page(matrix_a,m_size);
  matrix_b=matrix_page(matrix_b,m_size);
  matrix_c=matrix_page(matrix_c,m_size);

  //用随机整数 1-10 填充两个矩阵
  fill_matrix(matrix_a);
  fill_matrix(matrix_b);

  //在打印之前打印两个矩阵
  printf("Matrix A:\n---------\n");
  print_matrix(matrix_a, 2);
  printf("Matrix B:\n---------\n");
  print_matrix(matrix_b, 2);

  //为线程数据分配数组
  pthread_t *thrs;
  thr_args *args;
  if((thrs=malloc(sizeof(pthread_t) * (unsigned long) MATRIX_SIZE))==NULL||(args=malloc(sizeof(thr_args) * (unsigned long) MATRIX_SIZE))==NULL)
  {
    exit(EXIT_FAILURE);
  }

  //创建线程 0, 1, ..., N-1，并为它们提供一个包含其数据的结构
  for(int i=0;i<MATRIX_SIZE;i++)
  {
    args[i]=(thr_args) 
	{
      .a=matrix_a,
      .b=matrix_b,
      .c=matrix_c,
      .row=i
    };
    pthread_create(&thrs[i],NULL,(void *)&row_multiply,(void *)&args[i]);
  }

  //收集每个线程
  for(int j=0;j<MATRIX_SIZE;j++)
    pthread_join(thrs[j],NULL);

  //为每个线程分配的空闲资源
  if(thrs!=NULL)
  {
    free(thrs);
    thrs=NULL;
  }
  if(args!=NULL)
  {
    free(args);
    args=NULL;
  }

  //输出结果 
  printf("Result matrix:\n--------------\n");
  print_matrix(matrix_c,4);

  //归还分配的内存页
  matrix_unmap(matrix_a,m_size);
  matrix_unmap(matrix_b,m_size);
  matrix_unmap(matrix_c,m_size);
}
