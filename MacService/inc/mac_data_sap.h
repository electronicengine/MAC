#ifndef __mac_data_sap_H
#define __mac_data_sap_H

#include <stdio.h>
#include <stdlib.h>
#include "mac_command.h"
#include "messagetypes.h"
#include "mac_frames.h"

#define SETTRX_SIZE    1



struct MacDataSap;

struct MacDataSapOperations
{
    int (*dataTransmitRequest)(struct MacDataSap *Sap, ServiceMessage *MacMessage);
    int (*dataReceiveRequest)(struct MacDataSap *Sap, ServiceMessage *MacMessage);
};


struct MacDataSap
{
    struct MacCommand command;
    struct MacDataSapOperations ops;

};


void initMacDataSap(struct MacDataSap *Sap);
void deinitDataSap(struct MacDataSap *Sap);


#endif
