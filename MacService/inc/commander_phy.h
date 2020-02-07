#ifndef __commander_phy_H
#define __commander_phy_H

#include "phy_command.h"
#include "messagetypes.h"
#include "datamessages.h"
#include "managementmessages.h"
#include "phy_message_repo.h"

#define MAX_COMMAND_SIZE   32
#define TRYOUT              5

struct CommanderPhy;

struct CommanderPhyOperations
{
    int (*executeCommands)(struct CommanderPhy *Commander);
    int (*appendCommand)(struct CommanderPhy *Commander, struct PhyCommand *Command, ServiceMessage *Message);
    int (*clearCommands)(struct CommanderPhy *Commander);
};




struct CommanderPhy
{
    struct CommanderPhyOperations ops;
    struct PhyCommand   *commands[MAX_COMMAND_SIZE];

    ServiceMessage *messages[MAX_COMMAND_SIZE];

    ServiceMessage *phy_indication_data;

    int command_index;
    struct PhyMessageRepo rx_repo;


};



void initCommanderPhy(struct CommanderPhy *Commander);
void deinitCommanderPhy(struct CommanderPhy *Commander);



#endif
