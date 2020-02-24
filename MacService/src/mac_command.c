#include "mac_command.h"



void initMacCommand(struct MacCommand *Command)
{

    initPhyMessageRepo(&Command->phy_messages_repo);
    initCommanderPhy(&Command->commander_phy);
    initPhyDataSap(&Command->phy_data_sap);
    initPhyManagementSap(&Command->phy_management_sap);

}


