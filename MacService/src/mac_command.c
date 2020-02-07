#include "mac_command.h"



void initMacCommand(struct MacCommand *Command)
{

    initCommanderPhy(&Command->commander_phy);
    initPhyDataSap(&Command->phy_data_sap);
    initPhyManagementSap(&Command->phy_management_sap);

}


