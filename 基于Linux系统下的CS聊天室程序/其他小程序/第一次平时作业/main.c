#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include<string.h> 
#include "b2.h"
int main()
{
	clock_t sta,end;
	double time;
	sta=clock();
	int i; 
	for(i = 1;i < 40;i++)
	{
		//int res = fibonacci(50);
		printf("%d\n",fibonacci(i));
	};
	end=clock();
	time=(double)(end-sta)/CLOCKS_PER_SEC;
	printf("Time is %f s\n",time);
    return 0;
}

