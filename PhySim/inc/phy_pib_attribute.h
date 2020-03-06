#ifndef __phy_pib_attribute_H
#define __phy_pib_attribute_H


#include <inttypes.h>
#include <stdio.h>
#include <stdlib.h>



enum PhyPIBAttributeId
{

    cca_mode,
    owpan_coordinator,
    beacon_order,
    superframe_order

};



struct PhyPIBAttribute
{

    uint8_t     cca_mode;
    uint8_t     owpan_coordinatior;
    uint8_t     beacon_order;
    uint8_t     superframe_order;

};


#endif
