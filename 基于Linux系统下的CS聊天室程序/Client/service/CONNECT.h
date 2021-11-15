#ifndef _CONNECT_H
#define _CONNECT_H
#include "../src/Common.h"
#include <sys/types.h>
#include <linux/socket.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
void Connect(const char *host ,int port);
#endif