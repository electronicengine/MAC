#ifndef __phy_data_sap_H
#define __phy_data_sap_H

#include "phy_command.h"


struct PhyDataSap;

struct PhyDataSapOperations
{
    int (*dataTransmitRequest)(struct PhyDataSap *Sap, ServiceMessage *PhyMessage);
    int (*dataReceiveRequest)(struct PhyDataSap *Sap, ServiceMessage *PhyMessage);

};

struct PhyDataSap
{
    struct PhyDataSapOperations ops;
    struct PhyCommand command;

};




void initPhyDataSap(struct PhyDataSap *Sap);
void deinitPhyDataSap(struct PhyDataSap *Sap);


#endif
