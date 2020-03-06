#include "phy_message_repo.h"


static ServiceMessage *getServiceMessage(struct PhyMessageRepo *);

static PhyData *getPhyData(struct PhyMessageRepo *);

static PLMESetTRX *getPlmeSetTrx(struct PhyMessageRepo *);
static PLMECCA *getPlmeCca(struct PhyMessageRepo *);
static PLMEGet *getPlmeGet(struct PhyMessageRepo *);
static PLMESet *getPlmeSet(struct PhyMessageRepo *);
static PLMESwitch *getPlmeSwitch(struct PhyMessageRepo *);
static uint8_t *getRawData(struct PhyMessageRepo *);

static ServiceMessage *setServiceMessage(struct PhyMessageRepo *, uint8_t *);



void initPhyMessageRepo(struct PhyMessageRepo *Repo)
{

    Repo->setServiceMessage = setServiceMessage;
    Repo->getServiceMessage = getServiceMessage;
    Repo->getPhyData = getPhyData;
    Repo->getPlmeCca = getPlmeCca;
    Repo->getPlmeGet = getPlmeGet;
    Repo->getPlmeSet = getPlmeSet;
    Repo->getPlmeSetTrx = getPlmeSetTrx;
    Repo->getPlmeSwitch = getPlmeSwitch;

    Repo->getRawData = getRawData;
    Repo->index.service_message = 0;
    Repo->index.pd_data = 0;
    Repo->index.plme_cca = 0;
    Repo->index.plme_get = 0;
    Repo->index.plme_set = 0;
    Repo->index.plme_settrx = 0;
    Repo->index.plme_switch = 0;
    Repo->index.raw_data = 0;

}



void deinitPhyMessageRepo(struct PhyMessageRepo *Repo)
{

}


static uint8_t *getRawData(struct PhyMessageRepo *Repo)
{

    if(Repo->index.raw_data >= AVAILABLE_MESSAGE)
        Repo->index.raw_data = 0;
    return Repo->raw_data[Repo->index.raw_data++];
}



static ServiceMessage *getServiceMessage(struct PhyMessageRepo *Repo)
{
    if(Repo->index.service_message >= AVAILABLE_MESSAGE)
        Repo->index.service_message = 0;
    return &Repo->service_message[Repo->index.service_message++];
}



static PhyData *getPhyData(struct PhyMessageRepo *Repo)
{
    if(Repo->index.pd_data >= AVAILABLE_MESSAGE)
        Repo->index.pd_data = 0;
    return &Repo->pd_data[Repo->index.pd_data++];
}



static PLMESetTRX *getPlmeSetTrx(struct PhyMessageRepo *Repo)
{
    if(Repo->index.plme_settrx >= AVAILABLE_MESSAGE)
        Repo->index.plme_settrx = 0;
    return &Repo->plme_settrx[Repo->index.plme_settrx++];
}



static PLMECCA *getPlmeCca(struct PhyMessageRepo *Repo)
{
    if(Repo->index.plme_cca >= AVAILABLE_MESSAGE)
        Repo->index.plme_cca = 0;
    return &Repo->plme_cca[Repo->index.plme_cca++];
}



static PLMEGet *getPlmeGet(struct PhyMessageRepo *Repo)
{
    if(Repo->index.plme_get >= AVAILABLE_MESSAGE)
        Repo->index.plme_get = 0;
    return &Repo->plme_get[Repo->index.plme_get++];
}



static PLMESet *getPlmeSet(struct PhyMessageRepo *Repo)
{
    if(Repo->index.plme_set >= AVAILABLE_MESSAGE)
        Repo->index.plme_set = 0;
    return &Repo->plme_set[Repo->index.plme_set++];
}



static PLMESwitch *getPlmeSwitch(struct PhyMessageRepo *Repo)
{
    if(Repo->index.plme_switch >= AVAILABLE_MESSAGE)
        Repo->index.plme_switch = 0;
    return &Repo->plme_switch[Repo->index.plme_switch++];

}




static ServiceMessage *setServiceMessage(struct PhyMessageRepo *Repo, uint8_t *RawData)
{
    int index = 0;
    ServiceMessage *message = Repo->getServiceMessage(Repo);
    PLMECCA *plme_cca;
    PLMEGet *plme_get;
    PLMESet *plme_set;
    PLMESetTRX *plme_set_trx;
    PLMESwitch *plme_switch;
    PhyData *pd_data;


    message->header.type = RawData[index++];
    message->header.sub_type = RawData[index++];
    message->header.length = RawData[index++];
    message->header.length |= RawData[index++] << 8;


    switch(message->header.sub_type)
    {
        case cca:

            printf("set cca message\n");

            plme_cca = Repo->getPlmeCca(Repo);
            plme_cca->reason = RawData[index++];

            message->payload = (uint8_t *)plme_cca;

            break;

        case set_trx:

            printf("set trx message\n");

            plme_set_trx = Repo->getPlmeSetTrx(Repo);
            plme_set_trx->reason = RawData[index++];

            message->payload = (uint8_t *)plme_set_trx;

            break;

        case switch_state:

            plme_switch = Repo->getPlmeSwitch(Repo);

            plme_switch->reason = RawData[index++];
            plme_switch->length = RawData[index++];

            plme_switch->sw_bit_map = Repo->getRawData(Repo);
            memcpy(plme_switch->sw_bit_map, &RawData[index], plme_switch->length);
            index += plme_switch->length;

            plme_switch->dir = RawData[index++];

            message->payload = (uint8_t *)plme_switch;

            break;

        case get:

            plme_get = Repo->getPlmeGet(Repo);

            plme_get->reason = RawData[index++];
            plme_get->pib_attribute = RawData[index++];
            plme_get->pib_attribute_value = RawData[index++];

            message->payload = (uint8_t *)plme_get;

            break;

        case set:

            plme_set = Repo->getPlmeSet(Repo);

            plme_set->reason = RawData[index++];
            plme_set->pib_attribute = RawData[index++];
            plme_set->pib_attribute_value = RawData[index++];

            message->payload = (uint8_t *)plme_set;

            break;

        case transmit:

            printf("set transmit message\n");

            pd_data = Repo->getPhyData(Repo);

            pd_data->reason = RawData[index++];

            pd_data->payload = &RawData[index];
            index += message->header.length;

            pd_data->link_quality = RawData[index++];

            message->payload = (uint8_t *)pd_data;

            break;

        case receive:

            printf("set receive message\n");

            pd_data = Repo->getPhyData(Repo);

            pd_data->reason = RawData[index++];

            pd_data->payload = &RawData[index];
            index += message->header.length + MAC_FRAME_SIZE_OFFSET + MCSPDATA_SIZE_OFFSET;

            pd_data->link_quality = RawData[index++];

            message->payload = (uint8_t *)pd_data;

            break;

        default:
            break;
    }

     message->status_or_priorty = RawData[index++];

    return message;
}


