#ifndef __commander_mac_H
#define __commander_mac_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "mac_command.h"
#include "mac_management_sap.h"
#include "mac_data_sap.h"
#include "mac_message_repo.h"


#define MAX_COMMAND_SIZE   32



struct CommanderMac;



struct CommanderMacOperations
{
    int (*executeCommands)(struct CommanderMac *Commander);
    int (*appendCommand)(struct CommanderMac *Commander, struct MacCommand *Command, ServiceMessage *Message);
    int (*clearCommands)(struct CommanderMac *Commander);
};



struct CommanderMac
{
    struct CommanderMacOperations ops;
    struct MacCommand *commands[MAX_COMMAND_SIZE];
    ServiceMessage *messages[MAX_COMMAND_SIZE];
    int command_index;
    ServiceMessage *indication_message;

    struct MacMessageRepo rx_repo;

};



void initCommanderMac(struct CommanderMac *Commander);
void deinitCommanderMac(struct CommanderMac *Commander);



#endif
