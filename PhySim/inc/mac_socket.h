#ifndef __mac_socket_H
#define __mac_socket_H
#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "subject.h"
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>

#include "mac_message_repo.h"
#include "phy_message_repo.h"

#define MAX_TRANSFER_SIZE   1500
#define BACKLOG             10


struct MacSocket;

struct SocketOperations
{

    int (*getData)(struct MacSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*setData)(struct MacSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*closePort)(struct MacSocket *Socket);
    int (*openPort)(struct MacSocket *Socket);

};

struct MacSocket
{

    struct Subject subject;
    struct SocketOperations operations;

    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;

    struct MacMessageRepo mac_repo;
    struct PhyMessageRepo phy_repo;

    int server_sock;
    int client_sock;

};


int initMacSocket(struct MacSocket *Socket);
int deinitSocket(struct MacSocket *Socket);

//public functions
static int closePort(struct MacSocket *Socket);
static int openPort(struct MacSocket *Socket);
static int getData(struct MacSocket *Socket, uint8_t *TransmittedData, int Size);
static int setData(struct MacSocket *Socket, uint8_t *TransmittedData, int Size);

//private functions
static void createSocketThread(struct MacSocket *Socket);
static void *listenSocket(void *Socket);



#endif
