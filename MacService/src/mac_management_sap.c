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
    struct PhyCommand *command = &Sap->command.phy_management_sap.command;

    ServiceMessage *phy_message = repo->getServiceMessage(repo);
    PLMESet *plme_set = repo->getPlmeSet(repo);

    phy_message->header.type = phy_management;
    phy_message->header.sub_type = set;
    phy_message->header.length = sizeof(plme_set);

    plme_set->reason = request;
    plme_set->pib_attribute = Attribute;
    plme_set->pib_attribute_value = AttributeValue;

    phy_message->payload = (uint8_t *)plme_set;

    phy_message->status_or_priorty = 0;

    commander->ops.appendCommand(commander, command, phy_message);

}



static int startOwpan(struct MacManagementSap *Sap, ServiceMessage *Message)
{

    int ret;
    struct CommanderPhy *commander = &Sap->command.commander_phy;
    MLMEStart *mlme_start = (MLMEStart *)Message->payload;

    printf("Mac StartOwpan\n");

    makeSetPhyMessage(Sap, CCA_MODE, phy_cca_mode);
    makeSetPhyMessage(Sap, mlme_start->beacon_order, phy_beacon_order);
    makeSetPhyMessage(Sap, mlme_start->superframe_order, phy_superframe_order);
    makeSetPhyMessage(Sap, mlme_start->owpan_coordinator, phy_owpan_coordinator);

    ret = commander->ops.executeCommands(commander);

    commander->ops.clearCommands(commander);

    if(ret == FAIL)
    {
        printf("Owpan start has been failed!\n");
        return FAIL;
    }
    else
    {
        printf("Owpan has been started Succesfully\n");
        return SUCCESS;
    }



}



static int stopOwpan(struct MacManagementSap *Sap, ServiceMessage *Message)
{

}
