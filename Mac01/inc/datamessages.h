#ifndef __datamessages_H
#define __datamessages_H



#include "messagetypes.h"



#define SERVICE_MESSAGE_HEADER_SIZE     4
#define PAYLOAD_LINK_QUALITY_SIZE       1
#define PAYLOAD_REASON_SIZE             1
#define SERVICE_MESSAGE_STATUS_SIZE     1



typedef struct
{

    MassageReason   reason;
    uint8_t         *payload;
    uint8_t         link_quality;

}PhyData;



typedef struct
{

//    enum MassageReason   reason;
    uint64_t        destination_address : 48;
    uint64_t        source_address : 48;
    uint8_t         frame_handle;
    uint8_t         *frame;
    uint8_t         protect_enable;
    uint64_t        timestamp : 48;

}MCSPData;





#endif
