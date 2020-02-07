#include "phy_data_sap.h"

#include "datamessages.h"
#include "managementmessages.h"
#include "messagetypes.h"
#include "macframe.h"
#include <memory.h>



static int dataRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage);
static int execute(struct PhyCommand *Command, ServiceMessage *Message);



static int execute(struct PhyCommand *Command, ServiceMessage *Message)
{
    struct PhyDataSap *sap = container_of(Command, typeof(*sap), command);

    if(sap->ops.dataRequest(sap, Message) != -1)
        return DATA_COMMAND_RETURN;
    else
        return -1;

}



static int convertFrametoRaw(struct PhyDataSap *Sap, MCSPData *McspData, int *DataIndex)
{
    MacFrameFormat *mac_frame = (MacFrameFormat *)McspData->frame;
    MacFrameHeader *header = &mac_frame->header;
    FrameControl *control = &header->frame_control;
    POLLAck *pollack = &header->ack_information;
    SequenceControl *sequence_control = &header->sequence_control;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)(&control), sizeof(*control));
    *DataIndex += sizeof(*control);

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)(&pollack), sizeof(*pollack));
    *DataIndex += sizeof(*pollack);

    Sap->command.raw_data_fds[*DataIndex++] = header->receiver_address & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->receiver_address >> 8) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->receiver_address >> 16) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->receiver_address >> 24) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->receiver_address >> 32) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->receiver_address >> 40) & 0xff;

    Sap->command.raw_data_fds[*DataIndex++] = header->transmitter_address & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->transmitter_address >> 8) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->transmitter_address >> 16) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->transmitter_address >> 24) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->transmitter_address >> 32) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->transmitter_address >> 40) & 0xff;

    Sap->command.raw_data_fds[*DataIndex++] = header->auxiliary_address & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->auxiliary_address >> 8) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->auxiliary_address >> 16) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->auxiliary_address >> 24) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->auxiliary_address >> 32) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->auxiliary_address >> 40) & 0xff;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], (uint8_t *)sequence_control, sizeof(*sequence_control));
    *DataIndex += sizeof(*sequence_control);

//    Sap->command.raw_data_fds[*DataIndex++] = header->auxiliary_security_header;
    Sap->command.raw_data_fds[*DataIndex++] = (header->payload_length) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = (header->payload_length >> 8) & 0xff;

    memcpy(&Sap->command.raw_data_fds[*DataIndex], mac_frame->payload, header->payload_length);
    *DataIndex += header->payload_length;

    Sap->command.raw_data_fds[*DataIndex++] = (mac_frame->fcs) & 0xff;
    Sap->command.raw_data_fds[*DataIndex++] = ((mac_frame->fcs) >> 8) & 0xff;



}



static int convertMcspMessagetoRaw(struct PhyDataSap *Sap, PhyData *PD, int *DataIndex)
{

    MCSPData *mcsp_data = (MCSPData *)PD->payload;

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->reason;

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[0];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[1];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[2];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[3];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[4];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->destination_address[5];

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[0];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[1];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[2];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[3];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[4];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->source_address[5];

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->frame_handle;

    convertFrametoRaw(Sap, mcsp_data, DataIndex);

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->protect_enable;

    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[0];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[1];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[2];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[3];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[4];
    Sap->command.raw_data_fds[*DataIndex++] = mcsp_data->timestamp[5];

}



static int convertPhyMessagetoRaw(struct PhyDataSap *Sap, ServiceMessage *PhyMessage, int *DataIndex)
{
    PhyData *phy_data = (PhyData *)PhyMessage->payload;


    Sap->command.raw_data_fds[*DataIndex++] = phy_data->reason;

    convertMcspMessagetoRaw(Sap, phy_data, DataIndex);

    Sap->command.raw_data_fds[*DataIndex++] = phy_data->link_quality;


}



static int dataRequest(struct PhyDataSap *Sap, ServiceMessage *PhyMessage)
{

    struct Socket *sock = &Sap->command.sock;


    int data_index = 0;
    int ret;

    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.sub_type;
    Sap->command.raw_data_fds[data_index++] = PhyMessage->header.length & 0xff;
    Sap->command.raw_data_fds[data_index++] = ((PhyMessage->header.length >> 8) & 0xff);


    convertPhyMessagetoRaw(Sap, PhyMessage, &data_index);


    Sap->command.raw_data_fds[data_index++] = PhyMessage->status_or_priorty;

    ret = sock->ops.transmitData(sock, Sap->command.raw_data_fds, data_index);
    if(ret != -1)
        return sock->ops.receiveData(sock, Sap->command.raw_data_fds, data_index);

}



void initPhyDataSap(struct PhyDataSap *Sap)
{

    initPhyCommand(&Sap->command);

    Sap->command.ops.execute = execute;
    Sap->ops.dataRequest = dataRequest;

}



void deinitPhyDataSap(struct PhyDataSap *Sap)
{

}
