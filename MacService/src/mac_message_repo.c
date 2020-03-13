#include "mac_message_repo.h"
#include <stdio.h>

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

    Repo->index.mcsp_data = 0;
    Repo->index.mlme_associate = 0;
    Repo->index.mlme_disassociate = 0;
    Repo->index.mlme_get = 0;
    Repo->index.mlme_scan = 0;
    Repo->index.mlme_start = 0;
    Repo->index.mlme_stop = 0;


}



void deinitMacMessageRepo(struct MacMessageRepo *Repo)
{

}



static ServiceMessage *setServiceMessage(struct MacMessageRepo *Repo, ServiceMessage *PhyMessage)
{
    ServiceMessage *mac_message = Repo->getServiceMessage(Repo);
    MCSPData *mcsp_data = Repo->getMcspData(Repo);
    uint8_t *raw_data = (uint8_t *)((PhyData *)PhyMessage->payload)->payload;

    int data_index = 0;

    switch(PhyMessage->header.sub_type)
    {
        case receive:


            mac_message->header.type = mac_data;
            mac_message->header.sub_type = receive;
            mac_message->header.length  = PhyMessage->header.length;

            mcsp_data->reason = raw_data[data_index++];

            memcpy(mcsp_data->destination_address, &raw_data[data_index], sizeof(mcsp_data->destination_address));
            data_index += sizeof(mcsp_data->destination_address);

            memcpy(mcsp_data->source_address, &raw_data[data_index], sizeof(mcsp_data->source_address));
            data_index += sizeof(mcsp_data->source_address);

            mcsp_data->frame_handle = raw_data[data_index++];

            mcsp_data->msdu = &raw_data[data_index];
            data_index += mac_message->header.length + MAC_FRAME_SIZE_OFFSET;

            mcsp_data->protect_enable = raw_data[data_index++];

            memcpy(mcsp_data->timestamp, &raw_data[data_index], sizeof(mcsp_data->timestamp));

            mac_message->payload = (uint8_t *)mcsp_data;

            mac_message->status_or_priorty = PhyMessage->status_or_priorty;


            break;
        default:
            break;
    }

    return mac_message;
}



static ServiceMessage *getServiceMessage(struct MacMessageRepo *Repo)
{
    if(Repo->index.service_message >= AVAILABLE_MESSAGE)
        Repo->index.service_message = 0;
    return &Repo->service_message[Repo->index.service_message++];
}



static MCSPData *getMcspData(struct MacMessageRepo *Repo)
{
    if(Repo->index.mcsp_data >= AVAILABLE_MESSAGE)
        Repo->index.mcsp_data = 0;
    return &Repo->mcsp_data[Repo->index.mcsp_data++];
}



static MLMEAssociate *getMlmeAssociate(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_associate >= AVAILABLE_MESSAGE)
        Repo->index.mlme_associate = 0;
    return &Repo->mlme_associate[Repo->index.mlme_associate++];
}



static MLMEDisassociate *getMlmeDisassociate(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_disassociate >= AVAILABLE_MESSAGE)
        Repo->index.mlme_disassociate = 0;
    return &Repo->mlme_disassociate[Repo->index.mlme_disassociate++];
}



static MLMEGet *getMlmeGet(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_get >= AVAILABLE_MESSAGE)
        Repo->index.mlme_get = 0;
    return &Repo->mlme_get[Repo->index.mlme_get++];
}



static MLMEScan *getMlmeScan(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_scan >= AVAILABLE_MESSAGE)
        Repo->index.mlme_scan = 0;
    return &Repo->mlme_scan[Repo->index.mlme_scan++];
}



static MLMEStart *getMlmeStart(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_start >= AVAILABLE_MESSAGE)
        Repo->index.mlme_start = 0;
    return &Repo->mlme_start[Repo->index.mlme_start++];
}



static MLMEStop *getMlmeStop(struct MacMessageRepo *Repo)
{
    if(Repo->index.mlme_stop >= AVAILABLE_MESSAGE)
        Repo->index.mlme_stop = 0;
    return &Repo->mlme_stop[Repo->index.mlme_stop++];
}


