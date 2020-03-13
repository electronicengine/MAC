#ifndef __datatransmitter_H
#define __datatransmitter_H
#include <inttypes.h>
#include "observer.h"
#include <semaphore.h>
#include <pthread.h>
#include <time.h>
#include "macframe.h"
#include "datamessages.h"
#include "mac_socket.h"


#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

struct WirelessSocket;

struct DataTransmitterOperations
{

    void (*updateSocket)(struct Observer *Observer, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData);

};



struct DataTransmitter
{

    struct DataTransmitterOperations operations;
    struct Observer observer;

    MCSPData mcsp_data;
    MacFrameFormat mac_frame;
    PhyData phy_data;

};


void initDataTransmitter(struct DataTransmitter *Transmitter);

#endif
