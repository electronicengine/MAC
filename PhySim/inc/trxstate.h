#ifndef __trxstate_H
#define __trxstate_H
#include <inttypes.h>
#include "observer.h"
#include <semaphore.h>
#include <pthread.h>
#include <time.h>

#include "messagetypes.h"
#include "managementmessages.h"
#include "datamessages.h"


struct TRXStateOperations
{

    void (*spiDataUpdate)(struct Observer *Observer, struct UnixSocket *Socket, ServiceMessage *Message);

};

struct TRXState
{

    struct Observer observer;
};



void initTrxState(struct TRXState *trx);

#endif
