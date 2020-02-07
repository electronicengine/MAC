#include "mac_management_sap.h"



static int execute(struct MacCommand *Command, ServiceMessage *Message);



static int associate(struct MacManagementSap *Sap, ServiceMessage *Message);
static int disassociate(struct MacManagementSap *Sap, ServiceMessage *Message);
//static int set(struct MacCommandOperations *Command, ServiceMessage *Message);
//static int get(struct MacCommandOperations *Command, ServiceMessage *Message);
//static int scan(struct MacCommandOperations *Command, ServiceMessage *Message);
//static int start(struct MacCommandOperations *Command, ServiceMessage *Message);
//static int stop(struct MacCommandOperations *Command, ServiceMessage *Message);



void initMacManagementSap(struct MacManagementSap *Sap)
{
    initMacCommand(&Sap->Command);

    Sap->Command.ops.execute = execute;

}



void deintiMacManagementSap(struct MacManagementSap *Sap)
{

}



static int execute(struct MacCommand *Command, ServiceMessage *Message)
{

}
