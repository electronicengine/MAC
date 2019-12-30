#ifndef __servicetest_H
#define __servicetest_H



#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#include "mac.h"



struct ServiceTest;



struct ServiceTestOperations
{

    void (*testDataSap)(struct ServiceTest *Test);
    void (*testManagementSap)(struct ServiceTest *Test);
    void (*testMCSPRequest)(struct ServiceTest *Test);
    void (*testSocket)(struct ServiceTest *Test);

};



struct ServiceTest
{

    struct Observer                 observer;
    struct ServiceTestOperations    operations;
    struct MacService               *mac;

};


static void socketUpdate(struct Observer *Observer, struct UnixSocket *Socket,
                         ServiceMessageHeader *Header, uint8_t *TransmittedData);

static void testDataSap(struct ServiceTest *Test);
static void testManagementSap(struct ServiceTest *Test);
static void testMCSPRequest(struct ServiceTest *Test);
static void testSocket(struct ServiceTest *Test);

static void testCCA(struct ManagementSap *Sap);
static void testSetTrx(struct ManagementSap *Sap);


void initServiceTest(struct ServiceTest *Test, struct MacService *Mac);
void deinitServiceTest(struct ServiceTest *Sap);

#endif
