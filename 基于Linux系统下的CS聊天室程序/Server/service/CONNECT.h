#ifndef _CONNECT_H
#define _CONNECT_H
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
typedef struct 
{
    char type;
    char msg[1023];
} msg_t;
void Connect(int port);
#endif