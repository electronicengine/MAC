#ifndef __managementmessages_H
#define __managementmessages_H

#include "messagetypes.h"



typedef enum
{

    rx_on,
    tx_on,
    trx_off,
    force_trx_off

}SetRxTxStatus;



typedef struct
{

    MassageReason       reason;
    uint64_t            owpan_id :48;
    uint64_t            device_address : 48;
    uint16_t            short_address;

}MLMEAssociate;



typedef struct
{

    MassageReason       reason;
    uint64_t            owpan_id :48;
    uint64_t            device_address : 48;

}MLMEDisassociate;



typedef struct
{

    MassageReason       reason;
    uint8_t             attribute_id;
    uint8_t             attribute_value;

}MLMEGet;



typedef struct
{

    MassageReason       reason;
    uint16_t            scan_duration;
    uint8_t             *scan_list;

}MLMEScan;



typedef struct
{

    MassageReason       reason;
    uint16_t            delay;

}MLMEStart;



typedef struct
{

    MassageReason       reason;
    uint16_t            delay;

}MLMEStop;



typedef struct
{

    MassageReason   reason;

}PLMECCA;



typedef struct
{

    MassageReason       reason;
    uint8_t             pib_attribute;
    uint8_t             pib_attribute_value;

}PLMEGet;



typedef struct
{

    MassageReason       reason;
    uint8_t             pib_attribute;

}PLMESet;



typedef struct
{

    MassageReason       reason;

}PLMESetTRX;



typedef struct
{

    MassageReason       reason;
    uint8_t             length;
    uint8_t             *sw_bit_map;
    uint8_t             dir;

}PLMESwitch;

#endif
