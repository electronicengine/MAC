#ifndef __cca_H
#define __cca_H
#include <inttypes.h>
#include "observer.h"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#include "messagetypes.h"
#include "managementmessages.h"
#include "datamessages.h"


struct CCAOperations
{

    void (*spiDataUpdate)(struct Observer *Observer, struct UnixSocket *Socket, ServiceMessage *Message);

};

struct CCA
{
        struct CCAOperations operations;
        struct Observer observer;
};



void initCCA(struct CCA *cca);


#endif
