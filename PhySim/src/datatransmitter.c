#include <datatransmitter.h>
#include "phy.h"


static int extractMacFrame(struct DataTransmitter *Transmitter, uint8_t *McspPayload, int *Index)
{
    MacFrameHeader mac_frame_header;
    MacFrameFormat *mac_frame = &Transmitter->mac_frame;

    (*Index) = (*Index) + FRAMECONTROL_SIZE + POLLACK_SIZE;

    mac_frame_header.receiver_address[0] = McspPayload[(*Index)++];
    mac_frame_header.receiver_address[1] = McspPayload[(*Index)++];
    mac_frame_header.receiver_address[2] = McspPayload[(*Index)++];
    mac_frame_header.receiver_address[3] = McspPayload[(*Index)++];
    mac_frame_header.receiver_address[4] = McspPayload[(*Index)++];
    mac_frame_header.receiver_address[5] = McspPayload[(*Index)++];

    mac_frame_header.transmitter_address[0] = McspPayload[(*Index)++];
    mac_frame_header.transmitter_address[1] = McspPayload[(*Index)++];
    mac_frame_header.transmitter_address[2] = McspPayload[(*Index)++];
    mac_frame_header.transmitter_address[3] = McspPayload[(*Index)++];
    mac_frame_header.transmitter_address[4] = McspPayload[(*Index)++];
    mac_frame_header.transmitter_address[5] = McspPayload[(*Index)++];



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

    printf("MCSPData Source Addr: ");
    for(int i = 0; i<6; i++)
        printf("%02X-", mcsp_data->source_address[i]);

    printf("\n");

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


    printf("pd payload\n");

    for(int i=0; i<50; i++)
        printf("%02X-", pd->payload[i]);

    printf("\n");


//    extractMCSPData(Transmitter, pd->payload, &index);


//    pd->link_quality = PhyMessage->payload[index++];


    return 0;

}



static MacFrameFormat createMacFrame()
{
    MacFrameFormat mac_frame;

//    char *data = "hello world";

//    mac_frame.header.receiver_address = 0x121212121212;
//    mac_frame.header.transmitter_address = 0x212121212121;

//    mac_frame.header.payload_length = 11;
//    mac_frame.payload = (uint8_t *)data;
//    mac_frame.fcs = 0x20;

    return mac_frame;
}



static uint8_t *createIndicationPhyData(struct PhyMessageRepo *PhyMessageRepo, int *Index)
{


}



static uint8_t *createIndicationMessage(struct PhyMessageRepo *PhyRepo, struct MacMessageRepo *MacRepo, int *Index)
{

    uint8_t *raw_data = PhyRepo->getRawData(PhyRepo);
    ServiceMessage *message = PhyRepo->getServiceMessage(PhyRepo);

    message->header.type = phy_data;
    message->header.sub_type = receive;
    message->header.length =

    raw_data[(*Index)++] = message->header.type;
    raw_data[(*Index)++] = message->header.sub_type;
    raw_data[(*Index)++] = message->header.length & 0xff;
    raw_data[(*Index)++] = (message->header.length >> 8) & 0xff;

//    raw_data[(*Index)++] = pd->reason;
//    raw_data[(*Index)++] = 0;                         // for data payload
//    raw_data[(*Index)++] = pd->link_quality;

    raw_data[(*Index)++] = message->status_or_priorty;

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

    raw_data[(*Index)++] = 0;
    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



static void confirmDataTransmitRequest(struct DataTransmitter *Transmitter, struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;
    ServiceMessage *confirm_message = PhyMessage;
    int ret;


    printf("confirm dataTransmitterReqest\n");

    printDataTransmitRequest(Transmitter, PhyMessage);

    confirm_message->header.type = phy_data;
    confirm_message->header.sub_type = transmit;
    confirm_message->header.length = 1;
    confirm_message->payload = 0;
    confirm_message->status_or_priorty = 0;

    convertConfirmMessagetoRaw(phy_repo, confirm_message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void confirmDataReceiveRequest(struct DataTransmitter *Transmitter, struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;
    struct MacMessageRepo *mac_repo = &Socket->mac_repo;

    printf("confirm dataReceiveRequest\n");

    printDataTransmitRequest(Transmitter, PhyMessage);

    ((PhyData *)PhyMessage->payload)->reason = confirm;

    transmit_data = createIndicationMessage(phy_repo, mac_repo, &data_index);

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



