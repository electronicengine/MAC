#include "mac_service.h"
#include <string.h>


static int connectNetwork(struct MacService *Service, char *OwpanAddr);
static int createNetwork(struct MacService *Service, char *OwpanAddr);
static int sendData(struct MacService *Service, char *DestinationAddr, uint8_t *Data, uint16_t Lengt);
static int receiveData(struct MacService *Service, char *DestinationAddr);



static void convertArray(struct MacService *Service, const char *AddrStr, uint8_t *AddrArr)
{

    char buff[80];
    char delim[] = ".";

    strcpy(buff, AddrStr);

    char *ptr = strtok(buff, delim);

    for(int i=0; i<6; i++)
    {
         AddrArr[i] = atoi(ptr);
         ptr = strtok(NULL, delim);
    }
}



static int connectNetwork(struct MacService *Service, char *OwpanAddr)
{

    int ret;

    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    MLMEStart *mlme_start = Service->mac_message_repo.getMlmeStart(&Service->mac_message_repo);

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacManagementSap *sap = &Service->mac_management_sap;

    convertArray(Service, OwpanAddr, Service->mac_pib_attribute.owpan_addr);

    printf("Owpan is staring...\n");
    message->header.type = mac_management;
    message->header.sub_type = start;
    message->header.length = sizeof(MLMEStart);

    mlme_start->reason = request;

    memcpy(&mlme_start->owpan_addr, Service->mac_pib_attribute.owpan_addr, sizeof(mlme_start->owpan_addr));

    mlme_start->start_time = time(NULL);
    mlme_start->beacon_order = 13;
    mlme_start->superframe_order = 12;
    mlme_start->owpan_coordinator = FALSE;
    mlme_start->coord_realighment = FALSE;

    message->payload = (uint8_t *)mlme_start;
    message->status_or_priorty = 0;

    ret = commander->ops.appendCommand(commander, &sap->command, message);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    if(ret == FAIL)
    {
        printf("MLME Start Request is FAILED\n");
        return FAIL;
    }
    else
    {
        printf("MLME Start Request is SUCCESSFUL\n");
        return SUCCESS;
    }

}



static int createNetwork(struct MacService *Service, char *OwpanAddr)
{

    int ret;

    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    MLMEStart *mlme_start = Service->mac_message_repo.getMlmeStart(&Service->mac_message_repo);

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacManagementSap *sap = &Service->mac_management_sap;

    convertArray(Service, OwpanAddr, Service->mac_pib_attribute.owpan_addr);

    printf("Owpan is staring...\n");
    message->header.type = mac_management;
    message->header.sub_type = start;
    message->header.length = sizeof(MLMEStart);

    mlme_start->reason = request;

    memcpy(&mlme_start->owpan_addr, Service->mac_pib_attribute.owpan_addr, sizeof(mlme_start->owpan_addr));

    mlme_start->start_time = time(NULL);
    mlme_start->beacon_order = 13;
    mlme_start->superframe_order = 12;
    mlme_start->owpan_coordinator = TRUE;
    mlme_start->coord_realighment = FALSE;

    message->payload = (uint8_t *)mlme_start;
    message->status_or_priorty = 0;

    ret = commander->ops.appendCommand(commander, &sap->command, message);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    if(ret == FAIL)
    {
        printf("MLME Start Request is FAILED\n");
        return FAIL;
    }
    else
    {
        printf("MLME Start Request is SUCCESSFUL\n");
        return SUCCESS;
    }
}



static int sendData(struct MacService *Service, char *DestinationAddr, uint8_t *Data, uint16_t Length)
{

    int ret;
    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    MCSPData *mcsp_data = Service->mac_message_repo.getMcspData(&Service->mac_message_repo);

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacDataSap *sap = &Service->mac_data_sap;
    uint8_t destination_addr[6];
    uint64_t time_stamp = time(NULL);

    convertArray(Service, DestinationAddr, destination_addr);

    message->header.type = mac_data;
    message->header.sub_type = transmit;
    message->header.length = Length;

    mcsp_data->reason = request;

    memcpy(mcsp_data->destination_address, (uint8_t *)&destination_addr, sizeof(mcsp_data->destination_address));
    memcpy(mcsp_data->source_address, (uint8_t *)&Service->mac_pib_attribute.owpan_addr, sizeof(mcsp_data->source_address));

    mcsp_data->frame_handle = 12;
    mcsp_data->msdu = Data;
    mcsp_data->protect_enable = 3;

    memcpy(mcsp_data->timestamp, (uint8_t *)&time_stamp, sizeof(mcsp_data->timestamp));

    message->payload = (uint8_t *)mcsp_data;

    message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, &sap->command, message);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    if(ret == SUCCESS)
    {
        printf("MCSP Transmit Request is SUCCESSFUL\n");  
    }
    else
    {
        printf("MCSP Transmite Request is FAILED\n");
    }
}



static int convertRawtoMacFrame(uint8_t *RawData)
{

    MacFrameFormat mac_frame;
    MacFrameHeader *header = &mac_frame.header;
    FrameControl *control = &header->frame_control;
    POLLAck *poll_ack = &header->ack_information;
    SequenceControl *sequence_control = &header->sequence_control;

    int index = 0;

    index = index + FRAMECONTROL_SIZE + POLLACK_SIZE;

    header->receiver_address[0] = RawData[index++];
    header->receiver_address[1] = RawData[index++];
    header->receiver_address[2] = RawData[index++];
    header->receiver_address[3] = RawData[index++];
    header->receiver_address[4] = RawData[index++];
    header->receiver_address[5] = RawData[index++];

    header->transmitter_address[0] = RawData[index++];
    header->transmitter_address[1] = RawData[index++];
    header->transmitter_address[2] = RawData[index++];
    header->transmitter_address[3] = RawData[index++];
    header->transmitter_address[4] = RawData[index++];
    header->transmitter_address[5] = RawData[index++];

    header->auxiliary_address[0] = RawData[index++];
    header->auxiliary_address[1] = RawData[index++];
    header->auxiliary_address[2] = RawData[index++];
    header->auxiliary_address[3] = RawData[index++];
    header->auxiliary_address[4] = RawData[index++];
    header->auxiliary_address[5] = RawData[index++];

    index += SEQUENCECONTROL_SIZE;

    header->payload_length = RawData[index++];
    header->payload_length |= (RawData[index++] << 8) & 0xff;

    mac_frame.payload= &RawData[index];

    printf("mac frame payload %d: \n", header->payload_length);
    for(int i = 0; i< header->payload_length; i++)
        printf("%c", mac_frame.payload[i]);

    printf("\n");

    return 0;

}



static int receiveData(struct MacService *Service, char *SourceAddr)
{

    int ret;
    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    ServiceMessage *indication_message;

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacDataSap *sap = &Service->mac_data_sap;

    printf("\n\n\n, receiving...\n\n\n");

    message->header.type = mac_data;
    message->header.sub_type = receive;
    message->header.length = 1;
    message->payload = 0;
    message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, &sap->command, message);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    if(ret != FAIL)
    {

        printf("MCSP Receive Request is SUCCESSFUL\n");
        indication_message = commander->indication_message;

        MCSPData *mcsp_data = (MCSPData *)indication_message->payload;

        printf("Mcsp Source Addr: ");

        for(int i=0; i<6; i++)
            printf("%02X-", mcsp_data->source_address[i]);
        printf("\n");

    }
    else
    {
        printf("MCSP Receive Request is FAILED\n");
    }
}



void initMacService(struct MacService *Service, int Selection)
{

    Service->ops.connectNetwork = connectNetwork;
    Service->ops.createNetwork = createNetwork;
    Service->ops.receiveData = receiveData;
    Service->ops.sendData = sendData;

    initCommanderMac(&Service->commander_mac);
    initMacDataSap(&Service->mac_data_sap);
    initMacManagementSap(&Service->mac_management_sap);
    initMacMessageRepo(&Service->mac_message_repo);

    if(Selection == 1)
        createNetwork(Service, SERVER_OWPAN_ADDR);
    else if(Selection == 2)
        connectNetwork(Service, CLIENT_OWPAN_ADDR);

}



