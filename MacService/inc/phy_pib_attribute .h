#ifndef __phy_pib_attribute_H
#define __phy_pib_attribute_H


#include <inttypes.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <pthread.h>
#include "messagetypes.h"


enum PhyPIBAttributeId
{

    owpan_addr,
    owpan_coordinator,
    beacon_order,
    superframe_order

};

struct PhyPIBAttribute
{

    uint8_t     owpan_addr[6];
    uint8_t     owpan_coordinatior;
    uint8_t     beacon_order;
    uint8_t     superframe_order;

};






#endif
