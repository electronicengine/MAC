#ifndef __spisocket_H
#define __spisocket_H
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
#define SERVER_PATH         "/dev/mac01"
#define CLIENT_PATH         "/dev/physim"
#define BACKLOG             10


struct UnixSocket;

struct SocketOperations
{

    int (*getData)(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*setData)(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*closePort)(struct UnixSocket *Socket);
    int (*openPort)(struct UnixSocket *Socket);

};

struct UnixSocket
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


int initMacSocket(struct UnixSocket *Socket);
int deinitSocket(struct UnixSocket *Socket);

//public functions
static int closePort(struct UnixSocket *Socket);
static int openPort(struct UnixSocket *Socket);
static int getData(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size);
static int setData(struct UnixSocket *Socket, uint8_t *TransmittedData, int Size);

//private functions
static void createSocketThread(struct UnixSocket *Socket);
static void *listenSocket(void *Socket);



#endif
