#include "chainbase.h"
#include "cca.h"
#include "managementsap.h"
#include "phy.h"


static sem_t mutex;



static void confirmrequest(struct ManagementSap *Sap)
{
    sem_post(&mutex);
}



static void respondRequest(struct UnixSocket *Socket)
{
    printf("respondRequest CCA\n");

    struct ManagementSap *sap = &Socket->management_sap;

    sap->operations.createPLMECCAMessage(sap ,confirm);

    Socket->operations.setData(Socket, sap->transmitted.raw, sap->transmitted.index);
}



static void spiDataUpdate(struct Observer *Observer, struct UnixSocket *Socket,
                          ServiceMessageHeader *Header, uint8_t *TransmittedData)
{
    if(Header->type == phy_management && Header->sub_type == cca)
    {
        struct ManagementSap *sap = &Socket->management_sap;

        sap->operations.generatePLMECCAMessage(sap, TransmittedData);

        switch (sap->received.cca->reason)
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



static int CCAHandle(struct ChainBase *Base, uint8_t *Data, uint8_t Length)
{
    struct ManagementSap *sap = &Base->soket->management_sap;
    struct timespec ts;
    int ret;

    printf("CCAHandle\n");


    sap->operations.createPLMECCAMessage(sap, request);

    Base->soket->operations.setData(Base->soket,
                                    sap->transmitted.raw,
                                    sap->transmitted.index);

    clock_gettime(CLOCK_REALTIME, &ts);

    ts.tv_sec += 5;         // giving 5 seconds time out

    ret = sem_timedwait(&mutex, &ts);

    if(ret < 0)
    {
        printf("CCAHandle timeout error\n");
        return -1;
    }

    Base->operations.handle(Base->next_chain, Data, Length);
    Base->next_chain = 0; // clear after chain is done
    return 0;
}



void initCCA(struct CCA *cca, struct UnixSocket *Socket)
{

    cca->operations.CCAHandle = CCAHandle;
    cca->operations.spiDataUpdate = spiDataUpdate;

    initChainBase(&cca->base, Socket);
    cca->base.operations.handle = CCAHandle;

    initObserver(&cca->observer);
    cca->observer.operation.update = spiDataUpdate;

    sem_init(&mutex, 0, 0);

}



