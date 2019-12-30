#ifndef __messagetypes_H
#define __messagetypes_H



#include <inttypes.h>


#define SERVICE_MESSAGE_HEADER_SIZE     4
#define PAYLOAD_LINK_QUALITY_SIZE       1
#define PAYLOAD_REASON_SIZE             1
#define SERVICE_MESSAGE_STATUS_SIZE     1



typedef enum
{

    request,
    confirm,
    indication,
    response

}MassageReason;



typedef enum
{

    mac_data,
    mac_management,
    phy_data,
    phy_management

}ServiceMessageType;



typedef enum
{

    set,
    get,
    scan,
    start,
    stop,
    set_trx,
    switch_state,
    assotiate,
    disassotiate,
    cca,
    transmit

}ServiceMessageSubType;



typedef struct
{

    ServiceMessageType      type;
    ServiceMessageSubType   sub_type;
    uint16_t                length;

}ServiceMessageHeader;



typedef struct
{

    ServiceMessageHeader    *header;
    uint8_t                 *payload;
    uint8_t                 status_or_priorty;

}ServiceMessage;



#endif
