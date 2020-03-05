#ifndef __phy_command_H
#define __phy_command_H

#include "socket.h"
#include "messagetypes.h"
#include "phy_pib_attribute .h"

struct PhyCommand;

#define MAX_MESSAGE_SIZE 1500

#define container_of(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#define MESSAGE_HEADER_and_SIZE 5
#define PLME_CCA_SIZE   1
#define PLME_TRX_SIZE   1





struct PhyCommandOperations
{
    int (*execute)(struct PhyCommand *Command, ServiceMessage *Message);
};



struct PhyCommand
{
    struct PhyCommandOperations ops;
    struct Socket sock;
    uint8_t raw_data_fds[MAX_MESSAGE_SIZE]; // raw data for data sap
    uint8_t raw_data_fms[MAX_MESSAGE_SIZE]; // raw data for management sap

};


void initPhyCommand(struct PhyCommand *Command);
void deinitPhyCommand(struct PhyCommand *Command);


#endif
