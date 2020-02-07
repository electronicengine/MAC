#ifndef __mac_command_H
#define __mac_command_H

#include "commander_phy.h"
#include "phy_data_sap.h"
#include "phy_management_sap.h"

#include "messagetypes.h"
#include "managementmessages.h"
#include "datamessages.h"
#include "phy_message_repo.h"



struct MacCommand;


#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))



struct MacCommandOperations
{
    int (*execute)(struct MacCommand *Command, ServiceMessage *Message);
};



struct MacCommand
{
    struct MacCommandOperations ops;
    struct CommanderPhy commander_phy;
    struct PhyDataSap phy_data_sap;
    struct PhyManagementSap phy_management_sap;

    struct PhyMessageRepo phy_messages_repo;

    ServiceMessage *mac_indication_data;


};


void initMacCommand(struct MacCommand *Command);
void deinitMacCommand(struct MacCommand *Command);


#endif
