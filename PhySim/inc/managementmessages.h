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
    uint8_t             owpan_addr[6];
    uint32_t            start_time;
    uint32_t            beacon_order;
    uint8_t             superframe_order;
    uint8_t             owpan_coordinator;
    uint8_t             coord_realighment;
    uint8_t             coord_realignment_security_level;
    uint8_t             coord_realignment_keyid_mode;
    uint64_t            coord_realignment_keysource;
    uint8_t             coord_realignment_keyindex;
    uint8_t             beacon_security_level;
    uint8_t             beacon_key_id_mode;
    uint64_t            beacon_key_source;
    uint8_t             beacon_key_index;


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
    uint8_t             pib_attribute_value;

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
