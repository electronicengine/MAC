#ifndef __trxstate_H
#define __trxstate_H
#include <inttypes.h>
#include "chainbase.h"
#include "observer.h"
#include <semaphore.h>
#include <pthread.h>
#include <time.h>


struct TRXStateOperations
{
    int (*setRxOnHandle)(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length);
    int (*setTxOnHandle)(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length);
    int (*setTRxOffHandle)(struct ChainBase *Base, uint8_t *Data, uint8_t Length);

    void (*spiDataUpdate)(struct Observer *Observer, struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData);

};

struct TRXState
{

    struct ChainBase base;
    struct Observer observer;
};



//void trxInit(struct TRXState *trx, SetTrxSt)
void initTrxOff(struct TRXState *trx, struct UnixSocket *Socket);
void initTxOn(struct TRXState *trx, struct UnixSocket *Socket);
void initRxOn(struct TRXState *trx, struct UnixSocket *Socket);


#endif
