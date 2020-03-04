#ifndef __mac_pib_attribute_H
#define __mac_pib_attribute_H


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


struct MacPIBAttribute
{

    uint64_t    owpan_addr;
    uint8_t     owpan_coordinatior;
    uint8_t     beacon_order;
    uint8_t     superframe_order;

};






#endif
