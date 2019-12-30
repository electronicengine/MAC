#include "trxstate.h"
#include "mac.h"
#include "managementsap.h"

#include<pthread.h>
#include<stdio.h>
#include<string.h>
#include<pthread.h>
#include<stdlib.h>
#include<unistd.h>
#include<semaphore.h>


static sem_t mutex_rx_on;
static sem_t mutex_tx_on;
static sem_t mutex_trx_off;



static void respondRequest(struct UnixSocket *Socket)
{
    Socket->management_sap.operations.createPLMETrxMessage(&Socket->management_sap,
                                                           confirm, Socket->management_sap.received.message->status_or_priorty);
}



static void confirmRequest(struct ManagementSap *Sap)
{
    switch (Sap->received.message->status_or_priorty)
    {
        case rx_on:
                sem_post(&mutex_rx_on);
            break;
        case tx_on:
                sem_post(&mutex_tx_on);
            break;
        case trx_off:
                sem_post(&mutex_trx_off);
            break;

        default:
            break;
    }
}




static void spiDataUpdate(struct Observer *Observer, struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData)
{
    if(Header->type == phy_management && Header->sub_type == set_trx)
    {
        struct ManagementSap *sap = &Socket->management_sap;

        sap->operations.generatePLMETrxMessage(sap, TransmittedData);

        switch (sap->received.trx->reason)
        {

            case request:

                respondRequest(Socket);

                break;

            case confirm:

                confirmRequest(sap);

                break;

            case indication:

                break;


            case response:

                break;

            default:
                break;
        }

    }

}



static int setTRxOffHandle(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length)
{
    struct ManagementSap *sap = &Base->soket->management_sap;

    struct timespec ts;
    int ret;

    printf("setTRxOffHandle\n");

    sap->operations.createPLMETrxMessage(sap, request, trx_off);

    Base->soket->operations.setData
            (Base->soket, sap->transmitted.raw, sap->transmitted.index);

    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += 5;         // giving 5 seconds time out

    ret = sem_timedwait(&mutex_trx_off, &ts);

    if(ret < 0)
    {
        printf("setTRxOffHandle timeout error\n");
        return -1;
    }

    Base->operations.handle(Base, Chain->next_chain, Data, Length);
    Base->next_chain = 0; // clear after chain is done
    return 0;

}



static int setTxOnHandle(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length)
{

    struct ManagementSap *sap = &Base->soket->management_sap;
    struct timespec ts;
    int ret;

    printf("setTxOnHandle\n");


    sap->operations.createPLMETrxMessage
            (sap, request, tx_on);

    Base->soket->operations.setData(Base->soket,
                                    sap->transmitted.raw,
                                    sap->transmitted.index);


    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += 5;         // giving 5 seconds time out

    ret = sem_timedwait(&mutex_tx_on, &ts);

    if(ret < 0)
    {
        printf("setTxOnHandle timeout error\n");
        return -1;
    }

    Base->operations.handle(Base, Chain->next_chain, Data, Length); //next Chain
    Base->next_chain = 0; // clear after chain is done
    return 0;

}



static int setRxOnHandle(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length)
{

    struct ManagementSap *sap = &Base->soket->management_sap;
    struct timespec ts;
    int ret;

    printf("setRxOnHandle\n");

    sap->operations.createPLMETrxMessage
            (sap, request, rx_on);

    Base->soket->operations.setData(Base->soket,
                                    sap->transmitted.raw,
                                    sap->transmitted.index);

    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += 5;         // giving 5 seconds time out

    ret = sem_timedwait(&mutex_rx_on, &ts);

    if(ret < 0)
    {
        printf("setRxOnHandle timeout error\n");
        return -1;
    }


    Base->operations.handle(Base, Chain->next_chain, Data, Length);
    Base->next_chain = 0; // clear after chain is done
    return 0;


}



void initRxOn(struct TRXState *Trx, struct UnixSocket *Socket)
{


    initChainBase(&Trx->base, Socket);
    initObserver(&Trx->observer);

    Trx->base.operations.handle = setRxOnHandle;
    Trx->observer.operation.update = spiDataUpdate;

    sem_init(&mutex_rx_on, 0, 0);
}



void initTxOn(struct TRXState *Trx, struct UnixSocket *Socket)
{

    initChainBase(&Trx->base, Socket);
    initObserver(&Trx->observer);

    Trx->base.operations.handle = setTxOnHandle;
    Trx->observer.operation.update = spiDataUpdate;

    sem_init(&mutex_tx_on, 0, 0);

}



void initTrxOff(struct TRXState *Trx, struct UnixSocket *Socket)
{

    initChainBase(&Trx->base, Socket);
    initObserver(&Trx->observer);

    Trx->base.operations.handle = setTRxOffHandle;
    Trx->observer.operation.update = spiDataUpdate;

    sem_init(&mutex_trx_off, 0, 0);

}


