#ifndef __mac_management_sap_H
#define __mac_management_sap_H

#include "mac_command.h"
#include "messagetypes.h"

struct MacManagementSap;

struct MacManagementSapOperations
{
};


struct MacManagementSap
{
    struct MacCommandOperations ops;
    struct MacCommand Command;
};


void initMacManagementSap(struct MacManagementSap *Sap);
void deintiMacManagementSap(struct MacManagementSap *Sap);




#endif
