#ifndef __mac_H
#define __mac_H
#include <inttypes.h>

#include "datatransmitter.h"
#include "cca.h"
#include "trxstate.h"
#include "unixsocket.h"
#include "plmeset.h"
#include "subject.h"
#include "datamessages.h"
#include "managementmessages.h"

#define MAX_BEACON_PAYLOAD     1500

struct ChainBase;
struct DataSap;



struct MacService;


struct MACPIBAttributes
{
    uint16_t    ack_wait_duration;          //The maximum number of optical clocks to wait for an acknowledgment
    uint8_t     associated_owpan_coord;     //ndication of whether the device is associated to the OWPAN through the
    uint8_t     association_permit;         //Indication of whether a coordinator is currently allowing association.
    uint8_t     auto_request;               //Indication of whether a device automatically sends a data request command
    uint8_t     *beacon_payload;            //The contents of the beacon -payload.
    uint8_t     beacon_payload_length;      //The length, in octets, of the beacon payload.
    uint8_t     beacon_order;               //Specification of how often the coordinator transmits its beacon. If BO = 15, the coordinator will not transmit a periodic beacon.
    uint16_t    beacon_tx_time;             //The time that the device transmitted its last beacon frame,
    uint8_t     beacon_sequence_number;     //The sequence number added to the transmitted beacon frame.
    uint64_t    coordinator_extended_addr;  //he 64-bit address of the coordinator through which the device is -associated
    uint16_t    coordinator_short_addr;     //The 16-bit short address assigned to the coordinator through which the device is associated.
    uint8_t     data_sequence_number;       //The sequence number added to the transmitted data or MAC command frame.
    uint8_t     gts_permit;                 //TRUE if the coordinator is to accept GTS requests.
    uint8_t     max_be;                     //The maximum value of the backoff exponent,
    uint8_t     max_csma_backoffs;          //The maximum number of backoffs the unslotted random access algorithm
    uint8_t     max_frame_total_wait_time;  //The maximum number of optical clocksto wait either for a frame intended as a response to a data request frame or for a broadcast frame
    uint8_t     max_frame_reties;           //The maximum number of retries allowed after a transmission failure.
    uint8_t     max_ra_backoff;             //The maximum number of backoffs the unslotted random access algorithm will attempt before declaring a channel access failure.
    uint8_t     min_be;                     //The minimum value of the backoff exponent (BE) in the unslotted random access algorithm
    uint8_t     min_sifts_period;           //The minimum number of optical clocks forming a SIFS period.
    uint16_t    owpan_id;                   //The 16-bit identifier of the OWPAN on which the device is operating.
    uint16_t    response_wait_time;         //The maximum time, in multiples of aBaseSuperframeDuration, a device shall wait for a response command frame to be available following a request command frame
    uint8_t     rx_on_when_idle;            //Indication of whether the MAC sublayer is to enable its receiver during idle periods.
    uint8_t     security_enable;            //ndication of whether the MAC sublayer has security enabled.
    uint16_t    short_address;              //The 16-bit address that the device uses to communicate in the OWPAN.
    uint8_t     time_stamp_supported;       //Indication of whether the MAC sublayer supports the optional time stamping feature for incoming and outgoing data frames.
    uint16_t    transaction_persis_time;    //The maximum time (in unit periods) that a transaction isstored by a coordinator and indicated in its beacon.
    uint8_t     num_acks;                   //Maximum number of times not receiving ACKs to trigger fast link recovery procedure
    uint8_t     link_timeout;               //A timer initiated when the link recovery procedure is triggered. If the timer expires while the device has not received any fast link recovery response (FLR RSP) signal since the fast link recovery procedure is triggered, the device assumes that the link is broken and cannot be recovered.
    uint8_t     timestamp_offset;           //The location of the time stamp after the end of the preamble in optical clocks.
    uint8_t     symbol_rate;                //To adapt TX symbol rate versus RX frame rate variation when RX has the minimum frame rate below the symbol rate.
    uint8_t     ds_rate;                    //To control the length of a data subframe. Shorter data frame can support for longer distance transmission.
    uint8_t     tx_mode;                    //This attribute indicates the MAC transmission mode is visible or Invisible.
    uint8_t     tx_data_type;               //This attribute indicates the type of data to be transmitted.
    uint16_t    data_length;                //This attribute specify the length of the data to be transmitted
    uint8_t     coordination_time;          //This attribute indicates the number of superframes that consist of a coordination period
    uint8_t     neighbor_report_response;   //The maximum time, in multiples of aBaseSuperframeDuration,
    uint8_t     ageing_time;                //Ageing time for neighboring OWPAN monitoring records

};


struct MacOperations
{
    void (*sendData)(struct MacService *Mac, uint8_t *Data, uint16_t Length);
    void (*receiveData)(struct MacService *Mac, uint8_t *Data, uint8_t Length);
};



struct MacService
{

    struct ChainBase Chain;
    struct CCA  cca;
    struct DataTransmitter Transmitter;
    struct TRXState RxOn;
    struct TRXState TxOn;
    struct TRXState TrxOff;
    struct UnixSocket unix_sock;
    struct PLMESet  set;
    struct MacOperations operations;

};



void initMac(struct MacService *Mac);
void deinitMac(struct MacService *Mac);



#endif
