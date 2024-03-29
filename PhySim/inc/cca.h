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

    void (*updateSocket)(struct Observer *Observer, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData);

};

struct CCA
{
        struct CCAOperations operations;
        struct Observer observer;
};



void initCCA(struct CCA *cca);


#endif
