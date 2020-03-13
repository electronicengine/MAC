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

    void (*updateSocket)(struct Observer *Observer, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData);

};

struct TRXState
{

    struct Observer observer;
};



void initTrxState(struct TRXState *trx);

#endif
