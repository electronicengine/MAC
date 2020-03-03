#ifndef __datamessages_H
#define __datamessages_H



#include "messagetypes.h"



#define SERVICE_MESSAGE_HEADER_SIZE     4
#define PAYLOAD_LINK_QUALITY_SIZE       1
#define PAYLOAD_REASON_SIZE             1
#define SERVICE_MESSAGE_STATUS_SIZE     1

#define SERVICE_MESSAGE_OFFSET          5
#define PHYDATA_SIZE_OFFSET             2
#define MCSPDATA_SIZE_OFFSET            21


typedef struct
{

    MassageReason   reason;
    uint8_t         *payload;
    uint8_t         link_quality;

}PhyData;



typedef struct
{

    MassageReason   reason;
    uint8_t         destination_address[6]; //
    uint8_t         source_address[6];
    uint8_t         frame_handle;
    uint8_t         *frame;
    uint8_t         protect_enable;
    uint8_t         timestamp[6];

}MCSPData;





#endif
