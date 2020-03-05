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


struct SetPhyOperations
{

    void (*spiDataUpdate)(struct Observer *Observer, struct UnixSocket *Socket, ServiceMessage *Message);

};

struct SetPhy
{
        struct SetPhyOperations operations;
        struct Observer observer;
};



void initSetPhy(struct SetPhy *Setphy);


#endif
