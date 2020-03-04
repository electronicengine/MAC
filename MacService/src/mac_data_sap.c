#include "mac_data_sap.h"



static int execute(struct MacCommand *Command, ServiceMessage *MacMessage);
static int dataTransmitRequest(struct MacDataSap *Sap, ServiceMessage *MacMessage);
static int dataReceiveRequest(struct MacDataSap *Sap, ServiceMessage *MacMessage);



static int execute(struct MacCommand *Command, ServiceMessage *Message)
{

    struct MacDataSap *sap = container_of(Command, typeof(*sap), command);
    int ret;


    if(Message->header.sub_type == transmit)
    {
        ret = sap->ops.dataTransmitRequest(sap, Message);
        if(ret != FAIL)
            return DATA_COMMAND_RETURN;
        else
            return FAIL;

    }
    else if(Message->header.sub_type == receive)
    {
        ret = sap->ops.dataReceiveRequest(sap, Message);
        if(ret != FAIL)
        {
            Command->mac_indication_data = Command->commander_phy.phy_indication_data;
            return DATA_RECEIVE_RETURN;
        }
        else
            return FAIL;
    }

}



static int makeDataReceiveMessage(struct MacDataSap *Sap)
{

    struct PhyMessageRepo *repo = &Sap->command.phy_messages_repo;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    struct PhyCommand *command = &Sap->command.phy_data_sap.command;

    ServiceMessage *phy_message = repo->getServiceMessage(repo);
    PhyData *pd = repo->getPhyData(repo);

    phy_message->header.type = phy_data;
    phy_message->header.sub_type = receive;
    phy_message->header.length = 1;
    phy_message->payload = 0;
    pd->reason = request;
    pd->payload = 0;
    pd->link_quality = 0;
    phy_message->payload = (uint8_t *)pd;
    phy_message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, command, phy_message);

}



static int makeDataTransmitMessage(struct MacDataSap *Sap, uint8_t *Data, uint16_t Length)
{

    struct PhyMessageRepo *repo = &Sap->command.phy_messages_repo;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    struct PhyCommand *command = &Sap->command.phy_data_sap.command;

    ServiceMessage *phy_message = repo->getServiceMessage(repo);
    PhyData *pd = repo->getPhyData(repo);

    phy_message->header.type = phy_data;
    phy_message->header.sub_type = transmit;
    phy_message->header.length = Length;
    pd->reason = request;
    pd->payload = Data;
    pd->link_quality = 0;
    phy_message->payload = (uint8_t *)pd;
    phy_message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, command, phy_message);

}



static int makeSetTrxMessage(struct MacDataSap *Sap, SetRxTxStatus Status)
{

    static int creation_time = 0;

    struct PhyMessageRepo *repo = &Sap->command.phy_messages_repo;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    struct PhyCommand *command = &Sap->command.phy_management_sap.command;

    ServiceMessage *phy_message = repo->getServiceMessage(repo);
    PLMESetTRX  *plme_settrx = repo->getPlmeSetTrx(repo);

    phy_message->header.type = phy_management;
    phy_message->header.sub_type = set_trx;
    phy_message->header.length = SETTRX_SIZE;
    plme_settrx->reason = request;
    phy_message->payload = (uint8_t *)plme_settrx;
    phy_message->status_or_priorty = Status;

    commander->ops.appendCommand(commander, command, phy_message);

    if(creation_time >= AVAILABLE_MESSAGE)
        creation_time = 0;

}



static int makeCCAMessage(struct MacDataSap *Sap)
{

    struct PhyMessageRepo *repo = &Sap->command.phy_messages_repo;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    struct PhyCommand *command = &Sap->command.phy_management_sap.command;

    ServiceMessage *message = repo->getServiceMessage(repo);
    PLMECCA *plme_cca = repo->getPlmeCca(repo);

    message->header.type = phy_management;
    message->header.sub_type = cca;
    message->header.length = SETTRX_SIZE;

    plme_cca->reason = request;

    message->payload = (uint8_t *)plme_cca;
    message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, command, message);

}



static int extractMCSPData(struct MacDataSap *Sap, ServiceMessage *PhyMessage)
{



}



static int dataReceiveRequest(struct MacDataSap *Sap, ServiceMessage *MacMessage)
{

    int ret = 0;
    struct CommanderPhy *commander = &Sap->command.commander_phy;

    printf("MCSP Data Receive Request\n");

    makeSetTrxMessage(Sap, rx_on);
    makeDataReceiveMessage(Sap);

    ret = commander->ops.executeCommands(commander);
    commander->ops.clearCommands(commander);

    Sap->command.mac_indication_data = Sap->command.commander_phy.phy_indication_data;

    return ret;

}



static int dataTransmitRequest(struct MacDataSap *Sap, ServiceMessage *MacMessage)
{

    int ret = 0;
    struct CommanderPhy *commander = &Sap->command.commander_phy;

    printf("MCSP Data Transmit Request\n");

    makeSetTrxMessage(Sap, rx_on);
    makeCCAMessage(Sap);
    makeSetTrxMessage(Sap, tx_on);
    makeDataTransmitMessage(Sap, MacMessage->payload, MacMessage->header.length);
    makeSetTrxMessage(Sap, trx_off);

    ret = commander->ops.executeCommands(commander);

    commander->ops.clearCommands(commander);

    return ret;

}



void initMacDataSap(struct MacDataSap *Sap)
{

    initMacCommand(&Sap->command);

    Sap->ops.dataTransmitRequest = dataTransmitRequest;
    Sap->ops.dataReceiveRequest = dataReceiveRequest;

    Sap->command.ops.execute = execute;

}
