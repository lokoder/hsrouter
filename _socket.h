#ifndef _SOCKET_H_INCLUDED
#define _SOCKET_H_INCLUDED


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h> //inet_ntoa
#include <errno.h>
#include <string.h>
#include "_log.h"

int bind_and_listen(int port);
int connect_to(char *ip);

#endif

