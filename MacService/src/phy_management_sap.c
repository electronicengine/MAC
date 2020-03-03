#include "phy_management_sap.h"



static int execute(struct PhyCommand *Command, ServiceMessage *Message);
static int setTrx(struct PhyManagementSap *Sap, ServiceMessage *Message);
static int setCca(struct PhyManagementSap *Sap, ServiceMessage *Message);
static int setphy(struct PhyManagementSap *Sap, ServiceMessage *Message);
static int getphy(struct PhyManagementSap *Sap, ServiceMessage *Message);
static int switchChannel(struct PhyManagementSap *Sap, ServiceMessage *Message);
static int parseResponse(uint8_t *RawData);



static int execute(struct PhyCommand *Command, ServiceMessage *Message)
{

    ServiceMessageSubType type = Message->header.sub_type;
    struct PhyManagementSap *sap = container_of(Command, typeof(*sap), command);
    int ret = 0;

    switch (type)
    {
        case set_trx:

            if(setTrx(sap, Message) != -1)
                return MANAGEMENT_COMMAND_RETURN;
            else
                return FAIL;

            break;
        case cca:

            if(setCca(sap, Message) != -1)
                return MANAGEMENT_COMMAND_RETURN;
            else
                return FAIL;

            break;
        default:
            break;
    }

}



static int setCca(struct PhyManagementSap *Sap, ServiceMessage *Message)
{

    printf("cca\n");

    struct Socket *sock = &Sap->command.sock;
    PLMECCA *plme_cca = (PLMECCA *)Message->payload;
    int data_index = 0;
    int ret;

    Sap->command.raw_data_fms[data_index++] = Message->header.type;
    Sap->command.raw_data_fms[data_index++] = Message->header.sub_type;
    Sap->command.raw_data_fms[data_index++] = Message->header.length & 0xff;
    Sap->command.raw_data_fms[data_index++] = ((Message->header.length >> 8) & 0xff);

    Sap->command.raw_data_fms[data_index++] = plme_cca->reason;

    Sap->command.raw_data_fms[data_index++] = Message->status_or_priorty;

    sock->ops.transmitData(sock, Sap->command.raw_data_fms, data_index);

    if(ret != FAIL)
        ret = sock->ops.receiveData(sock, Sap->command.raw_data_fms, data_index);
        if(ret != FAIL)
            return SUCCESS;
        else
            return ret;


    return SUCCESS;

}



static int setTrx(struct PhyManagementSap *Sap, ServiceMessage *Message)
{

    if(Message->status_or_priorty== rx_on)
        printf("settrx_rx_on\n");
    if(Message->status_or_priorty == tx_on)
        printf("settrx_tx_on\n");
    if(Message->status_or_priorty == trx_off)
        printf("settrx_trx_off\n");

    int ret = 0;
    struct Socket *sock = &Sap->command.sock;
    PLMESetTRX *plme_settrx = (PLMESetTRX *)Message->payload;

    int data_index = 0;

    Sap->command.raw_data_fms[data_index++] = Message->header.type;
    Sap->command.raw_data_fms[data_index++] = Message->header.sub_type;
    Sap->command.raw_data_fms[data_index++] = Message->header.length & 0xff;
    Sap->command.raw_data_fms[data_index++] = ((Message->header.length >> 8) & 0xff);

    Sap->command.raw_data_fms[data_index++] = plme_settrx->reason;

    Sap->command.raw_data_fms[data_index++] = Message->status_or_priorty;

    ret = sock->ops.transmitData(sock, Sap->command.raw_data_fms, data_index);

    if(ret != FAIL)
    {
        ret = sock->ops.receiveData(sock, Sap->command.raw_data_fms, data_index);
        if(ret != FAIL)
            return SUCCESS;
        else
            return FAIL;
    }

    return SUCCESS;

}



void initPhyManagementSap(struct PhyManagementSap *Sap)
{

    initPhyCommand(&Sap->command);
    Sap->command.ops.execute = execute;

}



void deinitPhyManagementSap(struct PhyManagementSap *Sap)
{


}
