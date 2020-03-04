#include "mac_management_sap.h"



static int execute(struct MacCommand *Command, ServiceMessage *Message);



static int associate(struct MacManagementSap *Sap, ServiceMessage *Message);
static int disassociate(struct MacManagementSap *Sap, ServiceMessage *Message);
static int setMac(struct MacManagementSap *Sap, ServiceMessage *Message);
static int getMac(struct MacManagementSap *Sap, ServiceMessage *Message);
static int scanOwpan(struct MacManagementSap *Sap, ServiceMessage *Message);
static int startOwpan(struct MacManagementSap *Sap, ServiceMessage *Message);
static int stopOwpan(struct MacManagementSap *Sap, ServiceMessage *Message);



void initMacManagementSap(struct MacManagementSap *Sap)
{
    initMacCommand(&Sap->command);

    Sap->command.ops.execute = execute;
    Sap->ops.startOwpan = startOwpan;
    Sap->ops.stopOwpan = stopOwpan;

}



void deintiMacManagementSap(struct MacManagementSap *Sap)
{

}



static int execute(struct MacCommand *Command, ServiceMessage *Message)
{
    struct MacManagementSap *sap = container_of(Command, typeof(*sap), command);
    int ret;

    if(Message->header.sub_type == start)
    {
        ret = sap->ops.startOwpan(sap, Message);
        if(ret != FAIL)
            return MANAGEMENT_COMMAND_RETURN;
        else
            return FAIL;

    }
    else if(Message->header.sub_type == stop)
    {
        ret = sap->ops.stopOwpan(sap, Message);
        if(ret != FAIL)
            return MANAGEMENT_COMMAND_RETURN;
        else
            return FAIL;
    }
}



static int makeSetPhyMessage(struct MacManagementSap *Sap, uint8_t Attribute, uint8_t AttributeValue)
{

    struct PhyMessageRepo *repo = &Sap->command.phy_messages_repo;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    struct PhyCommand *command = &Sap->command.phy_data_sap.command;

    ServiceMessage *phy_message = repo->getServiceMessage(repo);
    PLMESet *plme_set = repo->getPlmeSet(repo);

    phy_message->header.type = phy_data;
    phy_message->header.sub_type = receive;
    phy_message->header.length = 1;
    phy_message->payload = 0;

    plme_set->reason = request;

    phy_message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, command, phy_message);

}



static int startOwpan(struct MacManagementSap *Sap, ServiceMessage *Message)
{



}


static int stopOwpan(struct MacManagementSap *Sap, ServiceMessage *Message)
{

}
