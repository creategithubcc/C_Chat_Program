#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>
#include <pthread.h>

//���ƾ���ߴ�
#define MATRIX_SIZE 5
 //���ά����һ��Ѱַ��ͨ����  
#define array(arr,i,j)arr[(int) MATRIX_SIZE*(int) i + (int) j]

void fill_matrix(int *matrix);
void print_matrix(int *matrix,int print_width);
int *matrix_page(int *matrix,unsigned long m_size);
void matrix_unmap(int *matrix,unsigned long m_size);
__attribute__ ((noreturn)) void row_multiply(void *row_args);

//���������ָ��
static int *matrix_a,*matrix_b,*matrix_c;

// �̵߳Ĳ����ṹ
typedef struct arg_struct
{
  int *a;
  int *b;
  int *c;
  int row;
} thr_args;

//�� 1-10����������������
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

//������� 
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

//ʹ�� mmap() �������ľ���ӳ�䵽�ڴ�ҳ��
int *matrix_page(int *matrix, unsigned long m_size)
{
  matrix=mmap(0,m_size, PROT_READ|PROT_WRITE,MAP_SHARED|MAP_ANONYMOUS,-1,0);
  //��� mmap()ʧ�ܾ��˳�
  if(matrix==(void *)-1)
  {
    exit(EXIT_FAILURE);
  }
  memset((void *)matrix,0,m_size);
  return matrix;
}

//���ڴ�ҳ��ȡ��ӳ������ľ���
void matrix_unmap(int *matrix,unsigned long m_size)
{
  //��� mmap()ʧ�ܾ��˳�
  if(munmap(matrix, m_size)==-1)
  {
    exit(EXIT_FAILURE);
  }
}

//�������������
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
  //���������ڴ��С
  unsigned long m_size=sizeof(int) * (unsigned long) (MATRIX_SIZE * MATRIX_SIZE);

  //�� matrix_a��matrix_b �� matrix_c ӳ�䵽�ڴ�ҳ
  matrix_a=matrix_page(matrix_a,m_size);
  matrix_b=matrix_page(matrix_b,m_size);
  matrix_c=matrix_page(matrix_c,m_size);

  //��������� 1-10 �����������
  fill_matrix(matrix_a);
  fill_matrix(matrix_b);

  //�ڴ�ӡ֮ǰ��ӡ��������
  printf("Matrix A:\n---------\n");
  print_matrix(matrix_a, 2);
  printf("Matrix B:\n---------\n");
  print_matrix(matrix_b, 2);

  //Ϊ�߳����ݷ�������
  pthread_t *thrs;
  thr_args *args;
  if((thrs=malloc(sizeof(pthread_t) * (unsigned long) MATRIX_SIZE))==NULL||(args=malloc(sizeof(thr_args) * (unsigned long) MATRIX_SIZE))==NULL)
  {
    exit(EXIT_FAILURE);
  }

  //�����߳� 0, 1, ..., N-1����Ϊ�����ṩһ�����������ݵĽṹ
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

  //�ռ�ÿ���߳�
  for(int j=0;j<MATRIX_SIZE;j++)
    pthread_join(thrs[j],NULL);

  //Ϊÿ���̷߳���Ŀ�����Դ
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

  //������ 
  printf("Result matrix:\n--------------\n");
  print_matrix(matrix_c,4);

  //�黹������ڴ�ҳ
  matrix_unmap(matrix_a,m_size);
  matrix_unmap(matrix_b,m_size);
  matrix_unmap(matrix_c,m_size);
}
