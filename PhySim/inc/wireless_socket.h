#ifndef __wireless_socket_H
#define __wireless_socket_H
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

#include "mac_message_repo.h"
#include "phy_message_repo.h"

#define SERVER_ADDR         "12.12.12.12.12.12"
#define CLIENT_ADDR         "21.21.21.21.21.21"
#define BACKLOG_WIRELESS    10


struct WirelessSocket;



typedef struct
{
    uint8_t     preamble;
    uint8_t     phy_header;
    uint16_t    length;
    uint8_t     *service_message;

}Beacon;



struct WirelessSocketOperations
{

    int (*getData)(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*setData)(struct WirelessSocket *Socket, uint8_t *TransmittedData, int Size);
    int (*closePort)(struct WirelessSocket *Socket);
    int (*openServerPort)(struct WirelessSocket *Socket);
    int (*openClientPort)(struct WirelessSocket *Socket);

};



struct WirelessSocket
{

    struct WirelessSocketOperations ops;

    struct sockaddr_un server_sockaddr;
    struct sockaddr_un client_sockaddr;

    struct MacMessageRepo mac_repo;
    struct PhyMessageRepo phy_repo;

    int server_sock;
    int client_sock;

};



int initWirelessSocket(struct WirelessSocket *Socket);
int deinitWirelessSocket(struct WirelessSocket *Socket);





#endif
