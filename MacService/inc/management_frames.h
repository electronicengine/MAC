#ifndef __management_frames_H
#define __management_frames_H



#include <inttypes.h>
#include "mac_frames.h"



typedef enum
{

    association_request_type,
    association_response_type,
    disassociation_notification_type,
    allien_signal_type,
    pmphymcs_type,
    hbphyrates_type,
    reserved_type,
    variable_element_containertype,
    probe_request_type,
    probe_response_type,
    attribute_change_request,
    attribute_change_response

}ManagementFrameTypes;



typedef struct
{

   uint8_t bitmap_width;        //Contains the width in octets of the subsequent Capability Bitmap field.
   uint8_t *capability_bitmap;  //A bitmap indicating a set of capabilities as given

}CapablityList;



typedef struct
{

    uint8_t pyhsical_id;    //The ID of the PHY for which the PHY MCS element is defined.
    uint8_t *mcs_element;   //PHY-specific element indicating supported optical clock rates and MCS.

}SupportedMCS;



typedef struct
{

    CapablityList capablity_list; //describing the supported capabilities of the device requesting association.
    SupportedMCS  supported_mcs;  //indicating which MCS(Modulation&Coding Scheme) are supported by the requesting device.

}AssociationRequest;



typedef struct
{

    uint8_t         status_code;            //indicates the result of the preceding association request.
    uint16_t        short_address;          //assigned to the device if the association was not denied.
    CapablityList   capability_list;        //
    SupportedMCS    supported_mcs;

}AssociationResponse;



typedef struct
{

   uint8_t reason_code; //conveys information about the disassociation of a device from an OWPAN

}DisassociationNotification;



typedef struct
{

    uint8_t signal_power;                   //optical power in dBm of the alien signal.
    uint8_t decodable : 1;                  //This bit shall only be set to one, if the alien signal is decodable by the PHY and MAC.
    uint8_t same_mac_mode : 1;              //This bit shall only be set to 1 if the received frame originates from an IEEE 802.15.13 OWPAN
    uint8_t owpan_id_clash : 1;             //This bit shall be set to one if the received frame originates from an OWPAN
    uint8_t reserved : 5;                   //reserved
    uint64_t foreign_owpan_id : 48;         //shall only be present, if the OWPAN ID Clash field was set to 0.
    uint64_t foreign_device_address : 48;   //shall only be present, if the Decodable field was set to 1

}AllienSignal;  //contains information about a signal that was received but identified as not originating from a device that is a member of the same OWPAN



typedef struct
{

    uint8_t clock_rates;        //indicating the set of supported optical clock rates (OCR).
    uint8_t modulation_formats; //ndicating a set of modulation formats. Bits 5 to 8 are reserved
    uint8_t line_coding;        //indicating a set of line codings. Bits 5 to 8 are reserved

}PmPhyMcs;  //holds a subset of supported MCS for the PM-PHY.



typedef struct
{

    uint8_t clock_rates;    //indicating the set of supported optical clock rates.

}HbPhyRates; //holds set of supported optical clock rates.



typedef struct
{

    uint64_t                owpan_id : 48;              //gives the ID for the OWPAN.
    SupportedRates          supportted_rates;           //have been standardized for each PHY in IEEE 802.15.13.
    ExtendedSupporttedRates extended_supported_rates;   //as standardized to handle more than eight data rates.

}ProbeRequest; //allows a device to send a request with information to a target coordinator in order to scan an area for existing IEEE 802.15.13 networks.



typedef struct
{

    uint16_t                timestamp;                  //allows synchronization between the devices in an OWPAN.
    uint16_t                beacon_interval;            //Each OWPAN can transmit Beacon frames at its own specific interval
    uint16_t                capability_information;     //is used to advertise the network’s capabilities. In this field, each bit is used as a flag to advertise a particular function of the network.
    uint16_t                owpan_id;                   //OWPAN ID field gives the ID for the OWPAN.
    SupportedRates          suppurted_rates;            //suppurted_rates
    ExtendedSupporttedRates extended_supported_rates;   //extended_supported_rates

}ProbeResponse; //If a Probe Request encounters a network with compatible parameters, the coordinator sends a Probe Response frame.



typedef struct
{

    uint16_t    attribute_id;   //indicates the attribute to be updated
    uint8_t     *new_value;     //The new value to assign to the attribute

}AttributeChangeRequest; //may be used by the coordinator of an OWPAN to change the PIB attribute value of an associated device.



typedef struct
{

    uint16_t    attribute_id;   //indicates the attribute to be updated
    uint8_t     *new_value;     //The new value to assign to the attribute
    uint8_t     status;         //the result of the former attribute change request.

}AttributeChangeResponse; //s transmitted from a device to the coordinator as a response to the Attribute Change Request element



#endif
