#ifndef __datatransmitter_H
#define __datatransmitter_H
#include <inttypes.h>
#include "observer.h"
#include "chainbase.h"
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

struct DataTransmitterOperations
{

    void (*spiDataUpdate)(struct Observer *Observer,  struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData);

    int (*dataTransmitterHandle)(struct ChainBase *Base, uint8_t *Data, uint8_t Length);

};



struct DataTransmitter
{

        struct DataTransmitterOperations operations;
        struct ChainBase base;
        struct Observer observer;

};


void initDataTransmitter(struct DataTransmitter *Transmitter, struct UnixSocket *Socket);

#endif
