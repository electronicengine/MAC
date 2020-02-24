#include "mac_message_repo.h"


static ServiceMessage *getServiceMessage(struct MacMessageRepo *);

static MCSPData *getMcspData(struct MacMessageRepo *);

static MLMEAssociate *getMlmeAssociate(struct MacMessageRepo *);
static MLMEDisassociate *getMlmeDisassociate(struct MacMessageRepo *);
static MLMEGet *getMlmeGet(struct MacMessageRepo *);
static MLMEScan *getMlmeScan(struct MacMessageRepo *);
static MLMEStart *getMlmeStart(struct MacMessageRepo *);
static MLMEStop *getMlmeStop(struct MacMessageRepo *);

static ServiceMessage *setServiceMessage(struct MacMessageRepo *, ServiceMessage *PhyMessage);


void initMacMessageRepo(struct MacMessageRepo *Repo)
{

    Repo->getServiceMessage = getServiceMessage;
    Repo->getMcspData = getMcspData;
    Repo->getMlmeAssociate = getMlmeAssociate;
    Repo->getMlmeDisassociate = getMlmeDisassociate;
    Repo->getMlmeGet = getMlmeGet;
    Repo->getMlmeScan = getMlmeScan;
    Repo->getMlmeStart = getMlmeStart;
    Repo->getMlmeStop = getMlmeStop;
    Repo->setServiceMessage = setServiceMessage;

}



void deinitMacMessageRepo(struct MacMessageRepo *Repo)
{

}



static ServiceMessage *setServiceMessage(struct MacMessageRepo *Repo, ServiceMessage *PhyMessage)
{
    ServiceMessage *mac_message = Repo->getServiceMessage(Repo);
    MCSPData *mcsp_data = Repo->getMcspData(Repo);
    uint8_t *raw_data = (uint8_t *) PhyMessage->payload;
    int data_index = 0;

    switch(PhyMessage->header.sub_type)
    {
        case transmit:

            mac_message->header.type = mac_data;
            mac_message->header.sub_type = transmit;
            mac_message->header.length  = PhyMessage->header.length;

            mcsp_data->reason = raw_data[data_index++];

            memcpy(mcsp_data->destination_address, &raw_data[data_index], sizeof(mcsp_data->destination_address));
            data_index += sizeof(mcsp_data->destination_address);

            memcpy(mcsp_data->source_address, &raw_data[data_index], sizeof(mcsp_data->source_address));
            data_index += sizeof(mcsp_data->source_address);

            mcsp_data->frame_handle = raw_data[data_index++];

            mcsp_data->frame = &raw_data[data_index];
            data_index += mac_message->header.length;

            mcsp_data->protect_enable = raw_data[data_index++];

            memcpy(mcsp_data->timestamp, &raw_data[data_index], sizeof(mcsp_data->timestamp));

            break;
        default:
            break;
    }

    return mac_message;
}



static ServiceMessage *getServiceMessage(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->service_message[index++];
}



static MCSPData *getMcspData(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mcsp_data[index++];
}



static MLMEAssociate *getMlmeAssociate(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_associate[index++];
}



static MLMEDisassociate *getMlmeDisassociate(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_disassociate[index++];
}



static MLMEGet *getMlmeGet(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_get[index++];
}



static MLMEScan *getMlmeScan(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_scan[index++];
}



static MLMEStart *getMlmeStart(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_start[index++];
}



static MLMEStop *getMlmeStop(struct MacMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->mlme_stop[index++];
}


