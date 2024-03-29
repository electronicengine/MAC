#ifndef __mac_management_sap_H
#define __mac_management_sap_H

#include "mac_command.h"
#include "messagetypes.h"
#include "phy_pib_attribute.h"


#define CCA_MODE 1

struct MacManagementSap;

struct MacManagementSapOperations
{
    int (*startOwpan)(struct MacManagementSap *Sap, ServiceMessage *Message);
    int (*stopOwpan)(struct MacManagementSap *Sap, ServiceMessage *Message);

};


struct MacManagementSap
{
    struct MacManagementSapOperations ops;
    struct MacCommand command;

};


void initMacManagementSap(struct MacManagementSap *Sap);
void deintiMacManagementSap(struct MacManagementSap *Sap);




#endif
