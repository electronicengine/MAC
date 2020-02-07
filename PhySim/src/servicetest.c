#include "servicetest.h"
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <time.h>
#include <assert.h>
#include <errno.h>
#include <signal.h>

static sem_t mutex;



void initServiceTest(struct ServiceTest *Test, struct PhyService *Phy)
{

    struct Subject *subject = &Phy->unix_socket.subject;

    Test->phy = Phy;
    initObserver(&Test->observer);

    Test->operations.testMCSPRequest = testMCSPRequest;
    Test->operations.testSocket = testSocket;
    Test->operations.testDataSap = testDataSap;
    Test->operations.testManagementSap = testManagementSap;
    Test->observer.operation.update = socketUpdate;



    sem_init(&mutex, 0, 0);

}



void deinitServiceTest(struct ServiceTest *Test)
{

}



static void socketUpdate(struct Observer *Observer, struct UnixSocket *Socket,
                         ServiceMessageHeader *Header, uint8_t *TransmittedData)
{

    const char *data ="hello world";
    int size = 11;

    for(int i=0; i<size; i++)
        if(data[i] != (char)TransmittedData[i])
            return;

    Socket->operations.setData(Socket, (uint8_t *)data, size);

    sem_post(&mutex);

}



static void testSocket(struct ServiceTest *Test)
{

    struct timespec     ts;
    struct UnixSocket   *socket = &Test->phy->unix_socket;
    struct Subject      *subject = &Test->phy->unix_socket.subject;
    int ret;

    const char *data = "hello world";
    uint16_t length = 11 ;

    subject->operations.registerObserver(subject, &Test->observer);

    sleep(2);
//    socket->operations.setData(socket, (uint8_t *)data, length);

//    clock_gettime(CLOCK_REALTIME, &ts);

//    ts.tv_sec += 5;         // giving 5 seconds time out

//    ret = sem_timedwait(&mutex, &ts);

//    if(ret < 0)
//        printf("testSocket: test error\n");
//    else
//        printf("**testSocket passed**\n");


    subject->operations.removeObserver(subject, &Test->observer);

}



static void testMCSPRequest(struct ServiceTest *Test)
{

    struct UnixSocket *socket = &Test->phy->unix_socket;
    struct ChainBase *chain = &Test->phy->Chain;
    struct PhyService *phy = Test->phy;

    const char *data = "hello world";
    uint16_t length = 11 ;


    chain->operations.addHandle(chain, &phy->RxOn.base);
    chain->operations.addHandle(chain, &phy->cca.base);
    chain->operations.addHandle(chain, &phy->TxOn.base);
    chain->operations.addHandle(chain, &phy->Transmitter.base);
    chain->operations.addHandle(chain, &phy->TrxOff.base);

    chain->operations.handle(chain, (uint8_t *)data, length);

}



static void testDataSap(struct ServiceTest *Test)
{

    struct DataSap *data_sap = &Test->phy->unix_socket.data_sap;
    const char *data = "hello world from Mac01";
    uint16_t length = 22 ;


    data_sap->operations.createPDMessage(data_sap, request, (uint8_t *)data, length);

//    for(int i = 0; i < data_sap->transmit.index; i++)
//        printf("%02X - ", data_sap->transmit.raw[i]);

//    printf("\n");

    data_sap->operations.generatePDMassage(data_sap, data_sap->transmit.raw, data_sap->transmit.index);

    if(data_sap->received.message->header->type != phy_data)
    {
        printf("type: data_sap test error\n" );
        return;
    }

    if(data_sap->received.message->header->sub_type != transmit)
    {
        printf("sub type: data_sap test error\n");
        return;
    }

    if(data_sap->received.data->reason != request)
    {
        printf("reason: data_sap test error\n");
        return;
    }

    for(int i = 0; i < data_sap->received.header->length; i++)
    {
        if(data_sap->received.data->payload[i] != data[i])
        {
            printf("message: data_sap test error\n");
            return;
        }
    }

    printf("**testDataSap is succesfull**\n");

}



static void testManagementSap(struct ServiceTest *Test)
{
    struct ManagementSap *sap = &Test->phy->unix_socket.management_sap;

    testCCA(sap);
    testSetTrx(sap);

}



void testCCA(struct ManagementSap *Sap)
{

    Sap->operations.createPLMECCAMessage(Sap, request);

//    for(int i=0; i<Sap->transmitted.index; i++)
//        printf("%02X-", Sap->transmitted.raw[i]);

//    printf("\n");

    Sap->operations.generatePLMECCAMessage(Sap, Sap->transmitted.raw);

    if(Sap->received.message->header->type != phy_management)
    {
        printf("type: testCCA test error\n" );
        return;
    }


    if(Sap->received.message->header->sub_type != cca)
    {
        printf("sub type: testCCA test error\n");
        return;
    }

    if(Sap->received.trx->reason != request)
    {
        printf("reason: testCCA test error\n");
        return;
    }

    if( Sap->received.message->status_or_priorty != 0)
    {
        printf("status: testCCA test error\n");
        return;
    }

    printf("**testCCA is successfull**\n");

}



void testSetTrx(struct ManagementSap *Sap)
{


    Sap->operations.createPLMETrxMessage(Sap, request, trx_off);

//    for(int i=0; i<Sap->transmitted.index; i++)
//        printf("%02X-", Sap->transmitted.raw[i]);

//    printf("\n");

    Sap->operations.generatePLMETrxMessage(Sap, Sap->transmitted.raw);

    if(Sap->received.message->header->type != phy_management)
    {
        printf("type: testSetTrx error\n" );
        return;
    }

    if(Sap->received.message->header->sub_type != set_trx)
    {
        printf("sub type: testSetTrx error %d\n", Sap->received.message->header->sub_type);
        return;
    }


    if(Sap->received.trx->reason != request)
    {
        printf("reason: testSetTrx error\n");
        return;
    }

    if(Sap->received.message->status_or_priorty != trx_off)
    {
        printf("Status : testSetTrx error \n");
        return;
    }

    printf("**testSetTrx is successfull**\n");

}
