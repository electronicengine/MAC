#ifndef __setphy_H
#define __setphy_H

#include <inttypes.h>
#include "observer.h"

#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#include "messagetypes.h"
#include "managementmessages.h"
#include "datamessages.h"



#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))


struct SetPhyOperations
{

    void (*updateSocket)(struct Observer *Observer, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData);

};

struct SetPhy
{
        struct SetPhyOperations operations;
        struct Observer observer;
};



void initSetPhy(struct SetPhy *Setphy);


#endif
