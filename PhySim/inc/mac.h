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
struct DataSap;



struct MacService;



struct MacOperations
{
    void (*sendData)(struct MacService *Mac, uint8_t *Data, uint16_t Length);
    void (*receiveData)(struct MacService *Mac, uint8_t *Data, uint8_t Length);
};



struct MacService
{

    struct ChainBase Chain;
    struct CCA  cca;
    struct DataTransmitter Transmitter;
    struct TRXState RxOn;
    struct TRXState TxOn;
    struct TRXState TrxOff;
    struct UnixSocket unix_sock;
    struct MacOperations operations;

};



void initMac(struct MacService *Mac);
void deinitMac(struct MacService *Mac);



#endif
