#ifndef __datatransmitter_H
#define __datatransmitter_H
#include <inttypes.h>
#include "observer.h"
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

struct DataTransmitterOperations
{

    void (*spiDataUpdate)(struct UnixSocket *Socket, ServiceMessage *Message);

};



struct DataTransmitter
{

        struct DataTransmitterOperations operations;
        struct Observer observer;

};


void initDataTransmitter(struct DataTransmitter *Transmitter);

#endif
