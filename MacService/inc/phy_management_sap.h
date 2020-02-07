#ifndef __phy_management_sap_H
#define __phy_management_sap_H

#include "phy_command.h"
#include "datamessages.h"
#include "managementmessages.h"


struct PhyManagementSap;


struct PhyManagementSapOperations
{

    int (*setTrx)(struct PhyManagementSap *Sap, SetRxTxStatus Status);
    int (*setPhy)(struct PhyManagementSap *Sap);
    int (*getPhy)(struct PhyManagementSap *Sap);
    int (*switchChannel)(struct PhyManagementSap *Sap);

};

struct PhyManagementSap
{
    struct PhyCommand command;
    struct PhyManagementSapOperations ops;

};



void initPhyManagementSap(struct PhyManagementSap *Sap);
void deinitPhyManagementSap(struct PhyManagementSap *Sap);

#endif
