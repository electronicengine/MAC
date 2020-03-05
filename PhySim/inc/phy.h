#ifndef __mac_H
#define __mac_H
#include <inttypes.h>

#include "datatransmitter.h"
#include "cca.h"
#include "setphy.h"
#include "trxstate.h"
#include "macsocket.h"
#include "subject.h"
#include "datamessages.h"
#include "managementmessages.h"

struct ChainBase;




struct PhyService;


struct PhyServiceOperations
{

};

struct PhyService
{
    struct SetPhy setphy;
    struct CCA  cca;
    struct DataTransmitter transmitter;
    struct TRXState trx;
    struct UnixSocket unix_socket;

    struct PhyServiceOperations operations;
};



void initPhy(struct PhyService *Phy);


#endif
