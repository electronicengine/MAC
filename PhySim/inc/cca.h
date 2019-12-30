#ifndef __cca_H
#define __cca_H
#include <inttypes.h>
#include "chainbase.h"
#include "observer.h"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>



struct CCAOperations
{

    int (*CCAHandle)(struct ChainBase *Base, uint8_t *Data, uint8_t Length);
    void (*spiDataUpdate)(struct Observer *Observer, struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData);

};

struct CCA
{
        struct CCAOperations operations;
        struct ChainBase base;
        struct Observer observer;
};



void initCCA(struct CCA *cca, struct UnixSocket *Socket);


#endif
