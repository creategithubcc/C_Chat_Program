#include <stdio.h>
#include "b2.h"
int fibonacci(int num)
{
	if(num < 1)
	{
		return 0;
	}else if(num <= 2)
	{
		return 1;
	}else{
		return (fibonacci(num-1)+fibonacci(num-2));
	}
	//return res;
}