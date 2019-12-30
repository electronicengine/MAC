#ifndef __mac_H
#define __mac_H
#include <inttypes.h>

#include "datatransmitter.h"
#include "cca.h"
#include "trxstate.h"
#include "unixsocket.h"
#include "subject.h"
#include "datamessages.h"
#include "managementmessages.h"

struct ChainBase;




struct PhyService;


struct PhyServiceOperations
{
    void (*sendData)(struct PhyService *Mac, uint8_t *Data, uint8_t Length);
    void (*receiveData)(struct PhyService *Mac, uint8_t *Data, uint8_t Length);
};

struct PhyService
{
    struct ChainBase Chain;
    struct CCA  cca;
    struct DataTransmitter Transmitter;
    struct TRXState RxOn;
    struct TRXState TxOn;
    struct TRXState TrxOff;
    struct UnixSocket unix_socket;

    struct PhyServiceOperations operations;
};



void initPhy(struct PhyService *Phy);
static void sendData(struct PhyService *Mac, uint8_t *Data, uint8_t Length);
static void receiveData(struct PhyService *Mac, uint8_t *Data, uint8_t Length);


#endif
