#ifndef __messagetypes_H
#define __messagetypes_H



#include <inttypes.h>


#define AVAILABLE_MESSAGE               10
#define SERVICE_MESSAGE_HEADER_SIZE     4
#define PAYLOAD_LINK_QUALITY_SIZE       1
#define PAYLOAD_REASON_SIZE             1
#define SERVICE_MESSAGE_STATUS_SIZE     1
#define MCSPDATA_SIZE_OFFSET            21
#define PHYDATA_SIZE_OFFSET             2


#define MAX_COMMAND_SIZE    32
#define MAX_MESSAGE_SIZE    1500

#define FAIL        -1
#define SUCCESS     0

#define DATA_RECEIVE_RETURN         3
#define DATA_COMMAND_RETURN         2
#define MANAGEMENT_COMMAND_RETURN   1






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
    transmit,
    receive

}ServiceMessageSubType;



typedef struct
{

    ServiceMessageType      type;
    ServiceMessageSubType   sub_type;
    uint16_t                length;

}ServiceMessageHeader;



typedef struct
{

    ServiceMessageHeader    header;
    uint8_t                 *payload;
    uint8_t                 status_or_priorty;

}ServiceMessage;



#endif
