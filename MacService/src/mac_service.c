#include "mac_service.h"



static int connectNetwork(struct MacService *Service);
static int createNetwork(struct MacService *Service);
static int sendData(struct MacService *Service, uint8_t *Data, uint16_t Length);
static int receiveData(struct MacService *Service);



static int connectNetwork(struct MacService *Service)
{

}



static int createNetwork(struct MacService *Service)
{

}



static int sendData(struct MacService *Service, uint8_t *Data, uint16_t Length)
{

    int ret;
    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    MCSPData *mcsp_data = Service->mac_message_repo.getMcspData(&Service->mac_message_repo);
    ServiceMessage *indication_message;

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacDataSap *sap = &Service->mac_data_sap;

    uint64_t destination_addr = 0x121212121212;
    uint64_t source_addr = 0x212121212121;
    uint64_t timestamp = 0;

    message->header.type = mac_data;
    message->header.sub_type = transmit;
    message->header.length = Length;

    mcsp_data->reason = request;

    memcpy(mcsp_data->destination_address, (uint8_t *)&destination_addr, sizeof(mcsp_data->destination_address));
    memcpy(mcsp_data->source_address, (uint8_t *)&source_addr, sizeof(mcsp_data->source_address));

    mcsp_data->frame_handle = 12;
    mcsp_data->frame = Data;
    mcsp_data->protect_enable = 3;

    memcpy(mcsp_data->timestamp, (uint8_t *)&timestamp, sizeof(mcsp_data->timestamp));

    message->payload = (uint8_t *)mcsp_data;

    message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, &sap->command, message);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    if(ret != FAIL)
        printf("MCSP Transmit Request is SUCCESSFUL\n");
    else
        printf("MCSP Transmite Request is FAILED\n");
}



static int receiveData(struct MacService *Service)
{

    int ret;
    ServiceMessage *message = Service->mac_message_repo.getServiceMessage(&Service->mac_message_repo);
    ServiceMessage *indication_message;

    struct CommanderMac *commander = &Service->commander_mac;
    struct MacDataSap *sap = &Service->mac_data_sap;

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
    }
    else
    {
        printf("MCSP Receive Request is FAILED\n");

    }


}



void initMacService(struct MacService *Service)
{

    Service->ops.connectNetwork = connectNetwork;
    Service->ops.createNetwork = createNetwork;
    Service->ops.receiveData = receiveData;
    Service->ops.sendData = sendData;

    initCommanderMac(&Service->commander_mac);
    initMacDataSap(&Service->mac_data_sap);
    initMacManagementSap(&Service->mac_management_sap);
    initMacMessageRepo(&Service->mac_message_repo);

}



