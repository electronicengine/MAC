#include "phy_data_sap.h"

#include "datamessages.h"
#include "managementmessages.h"
#include "messagetypes.h"
#include "mac_frames.h"
#include <memory.h>


static int dataReceiveRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage);
static int dataTransmitRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage);
static int execute(struct PhyCommand *Command, ServiceMessage *Message);




static int execute(struct PhyCommand *Command, ServiceMessage *Message)
{

    struct PhyDataSap *sap = container_of(Command, typeof(*sap), command);

    if(Message->header.sub_type == transmit)
        if(sap->ops.dataTransmitRequest(sap, Message) != FAIL)
            return DATA_COMMAND_RETURN;
        else
            return FAIL;
    else if(Message->header.sub_type == receive)
            if(sap->ops.dataReceiveRequest(sap, Message) != FAIL)
                return DATA_RECEIVE_RETURN;
            else
                return FAIL;
    else
        return FAIL;

}



static int convertFrametoRaw(struct PhyDataSap *Sap, uint8_t *MacFrame, int *DataIndex)
{

    MacFrameFormat *mac_frame = (MacFrameFormat *)MacFrame;

    MacFrameHeader *header = &mac_frame->header;
    FrameControl *control = &header->frame_control;
    POLLAck *pollack = &header->ack_information;
    SequenceControl *sequence_control = &header->sequence_control;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)(&control), FRAMECONTROL_SIZE);
    (*DataIndex) += FRAMECONTROL_SIZE;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)(&pollack), POLLACK_SIZE);
    (*DataIndex) += POLLACK_SIZE;

    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[2];;
    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[3];;
    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[4];;
    Sap->command.raw_data_fds[(*DataIndex)++] = header->receiver_address[5];;

    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[2];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[3];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[4];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->transmitter_address[5];

    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[2];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[3];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[4];
    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_address[5];

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)sequence_control, SEQUENCECONTROL_SIZE);
    *DataIndex += SEQUENCECONTROL_SIZE;

//    Sap->command.raw_data_fds[(*DataIndex)++] = header->auxiliary_security_header;
    Sap->command.raw_data_fds[(*DataIndex)++] = (header->payload_length) & 0xff;
    Sap->command.raw_data_fds[(*DataIndex)++] = (header->payload_length >> 8) & 0xff;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], mac_frame->payload, header->payload_length);
    *DataIndex += header->payload_length;

    Sap->command.raw_data_fds[(*DataIndex)++] = (mac_frame->fcs) & 0xff;
    Sap->command.raw_data_fds[(*DataIndex)++] = ((mac_frame->fcs) >> 8) & 0xff;
    Sap->command.raw_data_fds[(*DataIndex)++] = ((mac_frame->fcs) >> 16) & 0xff;
    Sap->command.raw_data_fds[(*DataIndex)++] = ((mac_frame->fcs) >> 24) & 0xff;


}



static int convertMcspMessagetoRaw(struct PhyDataSap *Sap, uint8_t *Payload, int *DataIndex)
{

    MCSPData *mcsp = (MCSPData *)Payload;

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->reason;

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[2];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[3];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[4];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->destination_address[5];

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[2];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[3];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[4];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->source_address[5];

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->frame_handle;

    convertFrametoRaw(Sap, mcsp->frame, DataIndex);

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->protect_enable;

    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[0];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[1];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[2];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[3];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[4];
    Sap->command.raw_data_fds[(*DataIndex)++] = mcsp->timestamp[5];


}



static int convertPhyMessagetoRaw(struct PhyDataSap *Sap, uint8_t *PhyMessage, int *DataIndex)
{

    PhyData *pd = (PhyData *)PhyMessage;

    Sap->command.raw_data_fds[(*DataIndex)++] = pd->reason;

    convertMcspMessagetoRaw(Sap, pd->payload, DataIndex);

    Sap->command.raw_data_fds[(*DataIndex)++] = pd->link_quality;


}



static int dataTransmitRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage)
{

    struct Socket *sock = &Sap->command.sock;

    int data_index = 0;
    int ret;

    printf("dataTransmitRequest\n");

    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.sub_type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.length & 0xff;
    Sap->command.raw_data_fds[data_index++] = ((PhyMessage->header.length >> 8) & 0xff);

    convertPhyMessagetoRaw(Sap, PhyMessage->payload, &data_index);

    Sap->command.raw_data_fds[data_index++] = PhyMessage->status_or_priorty;

    printf("Data Size: %d\n", data_index);


    ret = sock->ops.transmitData(sock, Sap->command.raw_data_fds, data_index);
    if(ret != FAIL)
        return sock->ops.receiveData(sock, Sap->command.raw_data_fds, data_index);

}



static int dataReceiveRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage)
{
    struct Socket *sock = &Sap->command.sock;
    PhyData *pd = (PhyData *)PhyMessage->payload;

    int data_index = 0;
    int ret;

    printf("dataReciveRequest\n");

    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.sub_type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.length & 0xff;
    Sap->command.raw_data_fds[data_index++] = ((PhyMessage->header.length >> 8) & 0xff);

    Sap->command.raw_data_fds[data_index++] = pd->reason;

    Sap->command.raw_data_fds[data_index++] = 0;

    Sap->command.raw_data_fds[data_index++] = pd->link_quality;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->status_or_priorty;

    ret = sock->ops.transmitData(sock, Sap->command.raw_data_fds, data_index);
    if(ret != FAIL)
        return sock->ops.receiveData(sock, Sap->command.raw_data_fds, MAX_MESSAGE_SIZE);
}



void initPhyDataSap(struct PhyDataSap *Sap)
{

    initPhyCommand(&Sap->command);

    Sap->command.ops.execute = execute;
    Sap->ops.dataTransmitRequest = dataTransmitRequest;
    Sap->ops.dataReceiveRequest = dataReceiveRequest;

}



void deinitPhyDataSap(struct PhyDataSap *Sap)
{

}
