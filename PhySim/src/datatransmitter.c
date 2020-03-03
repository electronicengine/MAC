#include <datatransmitter.h>
#include "phy.h"
#include <memory.h>


static int extractMacFrame(struct DataTransmitter *Transmitter, uint8_t *McspPayload, int *Index)
{
    MacFrameHeader mac_frame_header;
    MacFrameFormat *mac_frame = &Transmitter->mac_frame;

    int index = 0;


    index = index + FRAMECONTROL_SIZE + POLLACK_SIZE;

    mac_frame_header.receiver_address[0] = McspPayload[index++];
    mac_frame_header.receiver_address[1] = McspPayload[index++];
    mac_frame_header.receiver_address[2] = McspPayload[index++];
    mac_frame_header.receiver_address[3] = McspPayload[index++];
    mac_frame_header.receiver_address[4] = McspPayload[index++];
    mac_frame_header.receiver_address[5] = McspPayload[index++];

    mac_frame_header.transmitter_address[0] = McspPayload[index++];
    mac_frame_header.transmitter_address[1] = McspPayload[index++];
    mac_frame_header.transmitter_address[2] = McspPayload[index++];
    mac_frame_header.transmitter_address[3] = McspPayload[index++];
    mac_frame_header.transmitter_address[4] = McspPayload[index++];
    mac_frame_header.transmitter_address[5] = McspPayload[index++];

    mac_frame_header.auxiliary_address[0] = McspPayload[index++];
    mac_frame_header.auxiliary_address[1] = McspPayload[index++];
    mac_frame_header.auxiliary_address[2] = McspPayload[index++];
    mac_frame_header.auxiliary_address[3] = McspPayload[index++];
    mac_frame_header.auxiliary_address[4] = McspPayload[index++];
    mac_frame_header.auxiliary_address[5] = McspPayload[index++];

    index += SEQUENCECONTROL_SIZE;

    printf("ındex :%d\n", (index));
    mac_frame_header.payload_length = McspPayload[index++];
    mac_frame_header.payload_length |= (McspPayload[index++] << 8) & 0xff;


    mac_frame->payload = &McspPayload[index];

    printf("mac frame payload %d: \n", mac_frame_header.payload_length);
    for(int i = 0; i< mac_frame_header.payload_length; i++)
        printf("%c", mac_frame->payload[i]);

    printf("\n");


    *Index = index;
//    mac_frame->header.auxiliary_address[0] =  McspPayload



}



static int extractMCSPData(struct DataTransmitter *Transmitter, uint8_t *PhyPayload, int *Index)
{

    MCSPData *mcsp_data = &Transmitter->mcsp_data;

    mcsp_data->reason = PhyPayload[(*Index)++];

    mcsp_data->destination_address[0] = PhyPayload[(*Index)++];
    mcsp_data->destination_address[1] = PhyPayload[(*Index)++];
    mcsp_data->destination_address[2] = PhyPayload[(*Index)++];
    mcsp_data->destination_address[3] = PhyPayload[(*Index)++];
    mcsp_data->destination_address[4] = PhyPayload[(*Index)++];
    mcsp_data->destination_address[5] = PhyPayload[(*Index)++];

    printf("MCSPData Destination Addr: ");
    for(int i = 0; i<6; i++)
        printf("%02X-", mcsp_data->destination_address[i]);

    printf("\n");

    mcsp_data->source_address[0] = PhyPayload[(*Index)++];
    mcsp_data->source_address[1] = PhyPayload[(*Index)++];
    mcsp_data->source_address[2] = PhyPayload[(*Index)++];
    mcsp_data->source_address[3] = PhyPayload[(*Index)++];
    mcsp_data->source_address[4] = PhyPayload[(*Index)++];
    mcsp_data->source_address[5] = PhyPayload[(*Index)++];


    mcsp_data->frame_handle = PhyPayload[(*Index)++];

    mcsp_data->frame = &PhyPayload[(*Index)++];

    extractMacFrame(Transmitter, mcsp_data->frame, Index);

    mcsp_data->protect_enable = PhyPayload[(*Index)++];

    mcsp_data->timestamp[0] = PhyPayload[(*Index)++];
    mcsp_data->timestamp[1] = PhyPayload[(*Index)++];
    mcsp_data->timestamp[2] = PhyPayload[(*Index)++];
    mcsp_data->timestamp[3] = PhyPayload[(*Index)++];
    mcsp_data->timestamp[4] = PhyPayload[(*Index)++];
    mcsp_data->timestamp[5] = PhyPayload[(*Index)++];

}


static int printDataTransmitRequest(struct DataTransmitter *Transmitter, ServiceMessage *PhyMessage)
{

    PhyData *pd;

    int index = 0;

    pd = (PhyData *)PhyMessage->payload;


    extractMCSPData(Transmitter, pd->payload, &index);


    pd->link_quality = PhyMessage->payload[index++];


    return 0;

}



static int createIndicationMacFrame(uint8_t *RawData, int *Index)
{
    MacFrameFormat mac_frame;
    MacFrameHeader *mac_frame_header = &mac_frame.header;
    FrameControl *control = &mac_frame_header->frame_control;
    POLLAck *poll_ack = &mac_frame_header->ack_information;
    SequenceControl *sequence_control = &mac_frame_header->sequence_control;

    int index = 0;

    mac_frame_header->transmitter_address[0] = 0x21;
    mac_frame_header->transmitter_address[1] = 0x21;
    mac_frame_header->transmitter_address[2] = 0x21;
    mac_frame_header->transmitter_address[3] = 0x21;
    mac_frame_header->transmitter_address[4] = 0x21;
    mac_frame_header->transmitter_address[5] = 0x21;

    mac_frame_header->receiver_address[0] = 0x12;
    mac_frame_header->receiver_address[1] = 0x12;
    mac_frame_header->receiver_address[2] = 0x12;
    mac_frame_header->receiver_address[3] = 0x12;
    mac_frame_header->receiver_address[4] = 0x12;
    mac_frame_header->receiver_address[5] = 0x12;

    mac_frame_header->payload_length = 23;
    mac_frame.payload = "hello world from physim";
    mac_frame.fcs = 0x20;

    /***********************************************************************/

    memcpy(&RawData[index], (uint8_t *)control, FRAMECONTROL_SIZE);
    index += FRAMECONTROL_SIZE;

    memcpy(&RawData[index], (uint8_t *)poll_ack, POLLACK_SIZE);
    index += POLLACK_SIZE;

    RawData[index++] = mac_frame.header.receiver_address[0];
    RawData[index++] = mac_frame.header.receiver_address[1];
    RawData[index++] = mac_frame.header.receiver_address[2];
    RawData[index++] = mac_frame.header.receiver_address[3];
    RawData[index++] = mac_frame.header.receiver_address[4];
    RawData[index++] = mac_frame.header.receiver_address[5];

    RawData[index++] = mac_frame.header.transmitter_address[0];
    RawData[index++] = mac_frame.header.transmitter_address[1];
    RawData[index++] = mac_frame.header.transmitter_address[2];
    RawData[index++] = mac_frame.header.transmitter_address[3];
    RawData[index++] = mac_frame.header.transmitter_address[4];
    RawData[index++] = mac_frame.header.transmitter_address[5];

    RawData[index++] = mac_frame.header.auxiliary_address[0];
    RawData[index++] = mac_frame.header.auxiliary_address[1];
    RawData[index++] = mac_frame.header.auxiliary_address[2];
    RawData[index++] = mac_frame.header.auxiliary_address[3];
    RawData[index++] = mac_frame.header.auxiliary_address[4];
    RawData[index++] = mac_frame.header.auxiliary_address[5];

    memcpy(&RawData[index], (uint8_t *)sequence_control, SEQUENCECONTROL_SIZE);
    index += SEQUENCECONTROL_SIZE;

    RawData[index++] = mac_frame_header->payload_length & 0xff;
    RawData[index++] = (mac_frame_header->payload_length << 8)& 0xff;

    printf("payload len: %d\n", mac_frame_header->payload_length);
    memcpy(&RawData[index], mac_frame.payload, mac_frame_header->payload_length);
    index += mac_frame_header->payload_length;

    RawData[index++] = mac_frame.fcs & 0xff;
    RawData[index++] = (mac_frame.fcs << 8) & 0xff;
    RawData[index++] = (mac_frame.fcs << 16) & 0xff;
    RawData[index++] = (mac_frame.fcs << 24) & 0xff;

    (*Index) += index;

    return 0;
}


static int createIndicationMcspData(uint8_t *RawData, struct MacMessageRepo *MacRepo, int *Index)
{

    MCSPData *mcsp_data = MacRepo->getMcspData(MacRepo);
    int index = 0;

    mcsp_data->reason = indication;

    mcsp_data->destination_address[0] = 0x12;
    mcsp_data->destination_address[1] = 0x12;
    mcsp_data->destination_address[2] = 0x12;
    mcsp_data->destination_address[3] = 0x12;
    mcsp_data->destination_address[4] = 0x12;
    mcsp_data->destination_address[5] = 0x12;

    mcsp_data->source_address[0] = 0x21;
    mcsp_data->source_address[1] = 0x21;
    mcsp_data->source_address[2] = 0x21;
    mcsp_data->source_address[3] = 0x21;
    mcsp_data->source_address[4] = 0x21;
    mcsp_data->source_address[5] = 0x21;

    mcsp_data->frame_handle = 12;
    mcsp_data->protect_enable = 1;
    mcsp_data->timestamp[0] = 56;

    /************************************************************/

    RawData[index++] = mcsp_data->reason;

    RawData[index++] = mcsp_data->destination_address[0];
    RawData[index++] = mcsp_data->destination_address[1];
    RawData[index++] = mcsp_data->destination_address[2];
    RawData[index++] = mcsp_data->destination_address[3];
    RawData[index++] = mcsp_data->destination_address[4];
    RawData[index++] = mcsp_data->destination_address[5];

    RawData[index++] = mcsp_data->source_address[0];
    RawData[index++] = mcsp_data->source_address[1];
    RawData[index++] = mcsp_data->source_address[2];
    RawData[index++] = mcsp_data->source_address[3];
    RawData[index++] = mcsp_data->source_address[4];
    RawData[index++] = mcsp_data->source_address[5];

    RawData[index++] = mcsp_data->frame_handle;

    createIndicationMacFrame(&RawData[index], &index);

    RawData[index++] = mcsp_data->protect_enable;

    RawData[index++] = mcsp_data->timestamp[0];
    RawData[index++] = mcsp_data->timestamp[1];
    RawData[index++] = mcsp_data->timestamp[2];
    RawData[index++] = mcsp_data->timestamp[3];
    RawData[index++] = mcsp_data->timestamp[4];
    RawData[index++] = mcsp_data->timestamp[5];

    (*Index) += index;

    return 0;

}



static int createIndicationPhyData(uint8_t *RawData, struct PhyMessageRepo *PhyRepo, struct MacMessageRepo *MacRepo, int *Index)
{
    PhyData *pd = PhyRepo->getPhyData(PhyRepo);
    int index = 0;
    pd->reason = indication;

    RawData[index++] = pd->reason;

    createIndicationMcspData(&RawData[index], MacRepo, &index);

    RawData[index++] = pd->link_quality;

    *Index += index;

    return 0;

}



static uint8_t *createIndicationMessage(struct PhyMessageRepo *PhyRepo, struct MacMessageRepo *MacRepo, int *Index)
{

    uint8_t *raw_data = PhyRepo->getRawData(PhyRepo);
    ServiceMessage *message = PhyRepo->getServiceMessage(PhyRepo);
    int index = 0;

    message->header.type = phy_data;
    message->header.sub_type = receive;
    message->header.length = 23;

    message->status_or_priorty = 22;

    raw_data[index++] = message->header.type;
    raw_data[index++] = message->header.sub_type;
    raw_data[index++] = message->header.length & 0xff;
    raw_data[index++] = (message->header.length >> 8) & 0xff;

    createIndicationPhyData(&raw_data[index], PhyRepo, MacRepo, &index);

    raw_data[index++] = message->status_or_priorty;

    (*Index) += index;

    return raw_data;

}



static uint8_t *convertConfirmMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PhyData *pd = (PhyData *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    for(int i= 0; i<3; i++)
        raw_data[(*Index)++] = PhyMessage->payload[i];

    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



static void confirmDataTransmitRequest(struct DataTransmitter *Transmitter, struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;
    ServiceMessage *confirm_message = PhyMessage;
    PhyData *confirm_pd = (PhyData *)PhyMessage->payload;

    printf("confirm dataTransmitterReqest\n");

    printDataTransmitRequest(Transmitter, PhyMessage);

    confirm_message->header.type = phy_data;
    confirm_message->header.sub_type = transmit;
    confirm_message->header.length = 0;

    confirm_pd->reason = confirm;
    confirm_pd->payload = 0;
    confirm_pd->link_quality = 0;

    confirm_message->status_or_priorty = 0;

    transmit_data = convertConfirmMessagetoRaw(phy_repo, confirm_message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void confirmDataReceiveRequest(struct DataTransmitter *Transmitter, struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;
    struct MacMessageRepo *mac_repo = &Socket->mac_repo;

    printf("confirm dataReceiveRequest\n");

    transmit_data = createIndicationMessage(phy_repo, mac_repo, &data_index);

    printf("indication data %d: ", data_index);

    for(int i = 0; i<data_index; i++)
        printf("%d-", transmit_data[i]);

    printf("\n");

    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void spiDataUpdate(struct Observer *Obs, struct UnixSocket *Socket, ServiceMessage *Message)
{
    struct DataTransmitter *transmitter = container_of(Obs, typeof(*transmitter), observer);

    if(Message->header.type == phy_data && (Message->header.sub_type == transmit || Message->header.sub_type == receive))
    {
        switch (((PhyData *)Message->payload)->reason)
        {

            case request:

                if(Message->header.sub_type == transmit)
                    confirmDataTransmitRequest(transmitter, Socket, Message);
                if(Message->header.sub_type == receive)
                    confirmDataReceiveRequest(transmitter, Socket, Message);

                break;

            case confirm:


                break;

            case indication:


                break;

            default:
                break;
        }
    }
}



void initDataTransmitter(struct DataTransmitter *Transmitter)
{

    Transmitter->operations.spiDataUpdate = spiDataUpdate;

    initObserver(&Transmitter->observer);
    Transmitter->observer.operation.update = spiDataUpdate;

}



