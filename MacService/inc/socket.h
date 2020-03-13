#ifndef __socket_H
#define __socket_H


#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "messagetypes.h"

#define MAX_TRANSFER_SIZE   1500
#define SOCK_PATH           "/dev/mac01"
#define BACKLOG             10


struct Socket;

struct SocketOperations
{

    int (*openSocket)(struct Socket *Soc);
    int (*transmitData)(struct Socket *Soc, uint8_t *Data, uint16_t Length);
    int (*receiveData)(struct Socket *Soc, uint8_t *Data, uint16_t Length);
    int (*closeSocket)(struct Socket *Soc);

};


struct Socket
{

    struct SocketOperations ops;

    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;

    int server_sock;
    int client_sock;


};



int initSocket(struct Socket *Sock);
void deinitSocket(struct Socket *Sock);




#endif
