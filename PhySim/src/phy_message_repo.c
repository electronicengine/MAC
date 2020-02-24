#include "phy_message_repo.h"


static ServiceMessage *getServiceMessage(struct PhyMessageRepo *);

static PhyData *getPhyData(struct PhyMessageRepo *);

static PLMESetTRX *getPlmeSetTrx(struct PhyMessageRepo *);
static PLMECCA *getPlmeCca(struct PhyMessageRepo *);
static PLMEGet *getPlmeGet(struct PhyMessageRepo *);
static PLMESet *getPlmeSet(struct PhyMessageRepo *);
static PLMESwitch *getPlmeSwitch(struct PhyMessageRepo *);
static uint8_t *getRawData(struct PhyMessageRepo *);

uint8_t *convertMessagetoRaw(struct PhyMessageRepo *, ServiceMessage *PhyMessage, int *Index);

static ServiceMessage *setServiceMessage(struct PhyMessageRepo *, uint8_t *);



void initPhyMessageRepo(struct PhyMessageRepo *Repo)
{

    Repo->setServiceData = setServiceMessage;
    Repo->getServiceMessage = getServiceMessage;
    Repo->getPhyData = getPhyData;
    Repo->getPlmeCca = getPlmeCca;
    Repo->getPlmeGet = getPlmeGet;
    Repo->getPlmeSet = getPlmeSet;
    Repo->getPlmeSetTrx = getPlmeSetTrx;
    Repo->getPlmeSwitch = getPlmeSwitch;

    Repo->getRawData = getRawData;
    Repo->convertMessagetoRaw = convertMessagetoRaw;

}



void deinitPhyMessageRepo(struct PhyMessageRepo *Repo)
{

}



uint8_t *convertMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PhyData *pd = (PhyData *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    raw_data[(*Index)++] = pd->reason;

    memcpy(&raw_data[*Index], pd->payload, PhyMessage->header.length);
    (*Index) += PhyMessage->header.length;

    raw_data[(*Index)++] = pd->link_quality;

    return raw_data;

}



static uint8_t *getRawData(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return Repo->raw_data[index++];

}



static ServiceMessage *getServiceMessage(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->service_message[index++];
}



static PhyData *getPhyData(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->pd_data[index++];
}



static PLMESetTRX *getPlmeSetTrx(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->plme_settrx[index++];
}



static PLMECCA *getPlmeCca(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->plme_cca[index++];
}



static PLMEGet *getPlmeGet(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->plme_get[index++];
}



static PLMESet *getPlmeSet(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->plme_set[index++];
}



static PLMESwitch *getPlmeSwitch(struct PhyMessageRepo *Repo)
{
    static int index = 0;
    if(index > AVAILABLE_MESSAGE)
        index = 0;
    return &Repo->plme_switch[index++];

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

    printf("set service message sub type %d\n", message->header.sub_type);

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
            index += message->header.length;

            pd_data->link_quality = RawData[index++];

            message->payload = (uint8_t *)pd_data;

            break;

        default:
            break;
    }

     message->status_or_priorty = RawData[index++];

    return message;
}


