#include <datatransmitter.h>
#include "mac.h"
#include "datasap.h"



static sem_t mutex;


static void confirmrequest(struct DataSap *Sap)
{
    sem_post(&mutex);
}



static void respondRequest(struct UnixSocket *Socket)
{
    struct DataSap *sap = &Socket->data_sap;

    sap->operations.createPDMessage(sap ,confirm, "", 1);

    Socket->operations.setData(Socket, sap->transmit.raw, sap->transmit.index);
}



static void spiDataUpdate(struct Observer *Observer, struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData)
{

    if(Header->type == phy_data && Header->sub_type == transmit)
    {
        struct DataSap *sap = &Socket->data_sap;

        sap->operations.generatePDMassage(sap, TransmittedData, Header->length);

        switch (sap->received.data->reason)
        {

            case request:

                respondRequest(Socket);

                break;

            case confirm:

                confirmrequest(sap);

                break;

            case indication:


                break;

            default:
                break;
        }

    }

}



static int dataTransmitterHandle(struct ChainBase *Base, struct ChainBase *Chain, uint8_t *Data, uint8_t Length)
{

//    token = 0;
    struct DataSap *sap = &Base->soket->data_sap;
    struct timespec ts;
    int ret;

    printf("dataTransmitterHandle\n");

    sap->operations.createPDMessage(sap, request, Data, Length);

    Base->soket->operations.setData(Base->soket,
                                    sap->transmit.raw,
                                    sap->transmit.index);


    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += 5;         // giving 5 seconds time out

    ret = sem_timedwait(&mutex, &ts);

    if(ret < 0)
    {
        printf("dataTransmitterHandle timeout error\n");
        ret = -1;
    }


    Base->operations.handle(Base, Chain->next_chain, Data, Length);
    Base->next_chain = 0; // clear after chain is done
    return ret;
}



void initDataTransmitter(struct DataTransmitter *Transmitter, struct UnixSocket *Socket)
{

    Transmitter->operations.dataTransmitterHandle = dataTransmitterHandle;
    Transmitter->operations.spiDataUpdate = spiDataUpdate;

    initChainBase(&Transmitter->base, Socket);
    Transmitter->base.operations.handle = dataTransmitterHandle;

    initObserver(&Transmitter->observer);
    Transmitter->observer.operation.update = spiDataUpdate;

    sem_init(&mutex, 0, 0);

}



