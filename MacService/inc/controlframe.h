
#ifndef __controlframe_H
#define __controlframe_H


#include <inttypes.h>
#include "macframe.h"


enum ControlFrameTypes
{

    ack_type,
    block_ack_type,
    block_ack_request_type,
    mcs_request_type,
    gts_request_type,
    gts_descriptor_type,
    gts_descriptor_list_type,
    variable_element_container_type,
    super_frame_descriptor_type,
    random_access_type,
    bat_request_type,
    multi_ofe_feedback_type,
    hcm_allocation_type

};



typedef struct
{

    uint16_t sequence_number : 12;  //sequence number
    uint8_t reserved : 4;           //reserved

}ACK;   //is used by the receiver of an MPDU to signal successful reception of that MPDU to its transmitter



typedef struct
{
    uint16_t    first_sequence_number : 12; //sequence number of the first MPDU to be acknowledged.
    uint8_t     reserved : 4;               //reserved

}BlockACKRequest;   //is used by the transmitter of MPDU(s) to request an acknowledgment for the successful reception from the receiver.



typedef struct
{

    uint8_t     bitmap_width : 4;           //determines the width of the ACK Bitmap field in integer octets
    uint16_t    first_sequence_number : 12; //corresponding to the first bit in the subsequent ACK Bitmap field.
    uint8_t     *ack_bitmap;                //The actual acknowledgment information.

}BlockACK; //may be used by ta receiving device to signal the successful reception of multiple MPDUs to the transmitter in a single frame.



typedef struct
{

    uint8_t requested_mcs_id; //The ID of the requested MCS(Module & Coding Scheme)

}MCSRequest;    //is used by the prospective receiver of a transmission to request the usage of a certain MCS by the prospective transmitter.



typedef struct
{

   uint16_t beacon_number;                                  //Continuous number identifying each beacon frame and corresponding (hence current) superframe
   uint16_t total_superframe_slots;                         //The number of superframe slots in the current superframe.
   uint8_t  cap_slot_width;                                 //The number of superframe slots per CAP slot.
   uint8_t  cap_slots;                                      //The number of CAP slots included in the CAP of the current superframe.
   VariableElementContainer  variable_element_container;    //containing one or more elements.

}SuperframeDescriptor;



typedef struct
{
   uint16_t gts_start_slot;         //pecifies the first slot of the allocated GTS.
   uint16_t immediately_valid : 1;  //If the field is set to 1 the GTS becomes effective in the same superframe, the GTS Descriptor element was received.Otherwise, the GTS becomes effective in the following superframe.
   uint16_t gts_length : 15;        //specifies the duration of the GTS in superframe slots.

}GTSDescriptor;



typedef struct
{

    uint16_t queue_size_priority_0; //The number of octets currently queued in
    uint16_t queue_size_priority_2; //the corresponding outbound queue of the device.
    uint16_t queue_size_priority_1; //The actual value shall be rounded to the closes
    uint16_t queue_size_priority_3; //possible value.
    uint16_t queue_size_priority_4;
    uint16_t queue_size_priority_5;
    uint16_t queue_size_priority_6;
    uint16_t queue_size_priority_7;

}GTSRequest; //s transmitted by a device in order to inform the coordinator about the state of its MSDU queues.



typedef struct
{
    uint8_t         gts_descriptor_count;   //includes the number of GTS Descriptors that are subsequently included.
    GTSDescriptor   *gts_descriptors;       //contain one or multiple GTS Descriptor elements

}GTSDescriptorList; //holds multiple GTS Descriptor elements for a device in the beacon-enabled channel access mode.



typedef struct
{
    uint8_t *strength;  //Signal over noise ratio of the tap
    uint8_t *delay;     //Integer delay in the format specified in the Tap Format field

}TabDescriptor; //includes the information about a single tap



typedef struct
{
   uint8_t          pilot_symbol_number : 4;    //The number of distinct recognized OFEs.
   uint8_t          division_id : 4;            //describes the format for taps included in the child Tap Descriptor elements.
   uint8_t          number_of_tabs : 6;         //The measured noise floor [dBmV]
   uint8_t          reserved : 2;               //reserved
   TabDescriptor    *tab_descriptor;            //includes the information about a single tap

}OFEFeedbackDescriptor; //contains channel state information about a received signal from a given transmitter



typedef struct
{
   uint8_t  number_of_OFEs : 4;                     //The number of distinct recognized OFEs.
   uint8_t  tap_format : 4;                         //describes the format for taps included in the child Tap Descriptor elements.
   uint16_t noise_floor;                            //The measured noise floor [dBmV]
   OFEFeedbackDescriptor *ofe_feedback_descriptor;  //containing CSI for the channels between the device and each transmitting OFE.

}MultiOFEFeedback;      //is used to transfer multi-OFE channel feedback from a device to the coordinator of the OWPAN.



typedef struct
{

    uint8_t grouping : 4;       //contains the number of subcarriers in this group.
    uint8_t loaded_bits : 4;    //number of bits loaded on each subcarrier of the group.

}BATGroup; //contains information about a group of adjacent subcarriers, having the same number of bits loaded in a bit-loading capable PHY transmission.



typedef struct
{
    uint32_t    valid_bat_bitmap : 24;  //Specifies the BATs requested to be valid.The first bit of the bitmap corresponds to the BAT ID 8, while the last (i.e.. rightmost) bit corresponds to the BAT ID 31.
    uint8_t     updated_bat : 5;        //Specifies the ID of the runtime-defined BAT to be updated. Only values 8-31 are allowed.
    uint8_t     fec_block_size : 3;     //fec_block_size
    uint8_t     fec_code_rate : 3;      //Specifies the requested FEC coding rate.
    uint16_t    reserved : 13;          //reserved
    BATGroup    *bat_group;             //describing the modulation for the nth group of subcarriers.

}BATRequest;    //may be used by a receiving device using the HB-PHY to request usage of a certain bitloading and error-coding scheme from a prospective transmitter.



typedef struct
{

    uint16_t hcm_mask;  //The HCM rows assigned to the device. Each bit corresponds to an HCM row.

}HCMAllocation; //is sent by the coordinator to allocate one or more HCM rows to a device.



typedef struct
{
    uint64_t                timestamp;                 //field allows synchronization between the devices in an OWPAN
    uint16_t                random_access_interval;    //Each OWPAN can transmit Random Access frames at its own specific interval.
    uint16_t                capability_information;    //is used to advertise the network’s capabilities.
    uint64_t                owpan_id : 48;             //gives the ID for the OWPAN.
    SupportedRates          supported_rates;           //have been standardized for each PHY in IEEE 802.15.13.
    uint8_t                 *country;                  //The initial specifications were designed around the existing regulatory constraints in place in the major industrialized countries.
    ExtendedSupporttedRates extended_supported_rates;  //as standardized to handle more than eight data rates.

}RandomAccess; //contains information used to trigger the random access procedure in the non-beacon-enabled channel access mode.






#endif // __controlframe_H
