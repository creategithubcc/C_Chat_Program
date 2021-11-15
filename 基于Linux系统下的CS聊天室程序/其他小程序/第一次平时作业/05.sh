└─# gcc main.c b1.c b2.h -o nodll.out
                                                                             
└─# gcc -fPIC -c b1.c -o b3.o      
                                                                             
└─# gcc -shared b3.o -o libb4.so  
                                                                             
└─# gcc main.c -L. -lb4 -I. -o dll.out

export LD_LIBRARY_PATH=/home/linuxcc/桌面/test:LD_LIBRARY_PATH

./nodll.out

./dll.out
   