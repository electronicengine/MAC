
#ifndef __macframe_H
#define __macframe_H

#include <inttypes.h>


// This standard defines a single general MAC frame format,
// occurring in multiple variants depending on what information is carried in the frame.
// For discrimination and subsequent interpretation,
// each MAC frame starts with a Frame Control element,
// indicating a Type and Subtype of frame. Currently,
// three basic frame Types for the transmission of
// data, management and control information are supported.



enum MacFrameTypes
{

    data_frame_type,
    control_frame_type,
    management_frame_type

};



typedef struct
{

    uint8_t frame_version : 2;     //specifies the version of corresponding frame.
    uint8_t frame_type : 2;        //identify the function of the frame ref: Table 5
    uint8_t frame_subtype : 4;     //identify the function of the frame ref: Table 5
    uint8_t to_backhoul : 1;       //indicate if the transmission of frame involves coordinated topology. ref: table6
    uint8_t from_backhoul : 1;     //indicate if the transmission of frame involves coordinated topology. ref: table6
    uint8_t security_enabled : 1;  //indicate security needed
    uint8_t ack_request : 1;       //specifies whether an acknowledgment is required from the recipient device
    uint8_t nonbeacon_enabled : 1; //Specifies whether the transmitting device operates in the non-beacon-enabled mode
    uint8_t short_addressing : 1;  //Indicates whether short addresses are used
    uint8_t last_fragment : 1;     //set to ‘1’ if the frame is the last fragment
    uint8_t reserved : 1;          //reserved field

}FrameControl;



typedef struct{

    uint8_t        compressed_device_address : 7;   //contain the short address of the device which transmitted the packet
    uint16_t       sequence_number : 7;             //identify the sequence number of the packet
    uint8_t        ack : 2;                         //bit 0 set to '1' when a packet is being acknowledged with the current frame
                                                    //bit 1 set to '1' when the last Beacon frame reception is being acknowledged
}POLLAck;



typedef struct
{
    uint8_t fragment_number : 4;    //for packet fragmentation support
    uint16_t sequence_number : 12;  //identify the packet sequence number

}SequenceControl;



typedef struct
{
    FrameControl    frame_control;                  //defining the frame type, addressing fields, and other control flags
    POLLAck         ack_information;                //contains the information necessary to identify the MSDU sequence number and the station which transmitted the acknowledged information
    uint64_t        receiver_address : 48;          //MAC individual or group address that identifies the intended immediate recipient device(s)
    uint64_t        transmitter_address : 48;       //MAC address that identifies the device that has transmitted the MPDU contained in the frame body field.
    uint64_t        auxiliary_address : 48;         //offer extra information that is needed for data frame transmission. It could be OWPAN ID, source address or destination address,
    SequenceControl sequence_control;               //identify the MSDU sequence number
    uint8_t         *auxiliary_security_header;     //a variable length and specifies information required for security processing

}MacFrameHeader;



typedef struct
{
    MacFrameHeader  header;
    uint16_t        payload_length;  // bence eklenmesi gereken değişken
    uint8_t         *payload;
    uint32_t        fcs;

}MacFrameFormat;



typedef struct
{
   uint64_t destination_mac_address : 48;   //destination address of the MSDU.
   uint64_t source_address : 48;            //source address of the MSDU.
   uint16_t msdu_length;                    //contains the length of the subsequent MSDU in octets.
   uint8_t *msdu;                           //contains the MSDU to be aggregated.
   uint32_t padding : 24;                   //contains 0, 1, 2 or 3 octets in order to make the total length of the element a multiple of 4 octets. The number of padded octets can be derived based on the MSDU Length field.

}MSDUAggregation;       //serves the aggregation of multiple MSDUs in one A-MSDU data frame.



typedef struct
{

    uint8_t element_id;                 //element id
    uint8_t length;                     //rates
    uint8_t *supported_rates;           //supported rates

}SupportedRates; //have been standardized for each PHY in IEEE 802.15.13.



typedef struct
{

    uint8_t element_id;                 //element id
    uint8_t length;                     //rates
    uint8_t *extended_supported_rates;  //extended supported rates

}ExtendedSupporttedRates; //was standardized to handle more than eight data rates.



typedef struct
{

    uint32_t type;      //The type of the subsequent element. This field has 2 octets width.
    uint32_t length;    //contains the length of the subsequent element in octets and is 2 octets wide.
    uint8_t *element;   //The contained element indicated by the Type field as defined in the respective clause.

}VariableElementContainer; // comprises one or multiple other elements. For each element, a type, an optional length and the actual element are included.



#endif // __macframe_H


