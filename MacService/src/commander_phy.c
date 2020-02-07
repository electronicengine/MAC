#include "commander_phy.h"



void initCommanderPhy(struct CommanderPhy *Commander);
void deinitCommanderPhy(struct CommanderPhy *Commander);
static MassageReason parseData(struct CommanderPhy *Commander, uint8_t * Data);
static MassageReason parsePlmeSetTrx(struct CommanderPhy *Commander, uint8_t *Data);
static MassageReason parsePhyData(struct CommanderPhy *Commander, uint8_t *Data);



static int checkMessage(struct CommanderPhy *Commander, ServiceMessage *Message)
{

    switch(Message->header.sub_type)
    {
        case cca:

            if(((PLMECCA *)Message->payload)->reason == confirm)
                return 0;

            else
                return -1;

        case set_trx:

            if(((PLMESetTRX *)Message->payload)->reason == confirm)
                return 0;

            else
                return -1;

        case transmit:

            if(((PhyData *)Message->payload)->reason == confirm)
                return 0;
            else if(((PhyData *)Message->payload)->reason == indication ||
                    ((PhyData *)Message->payload)->reason == response)
            {
                Commander->phy_indication_data = Message;
                return 0;
            }
            else
                return -1;

        case get:

            if(((PLMEGet *)Message->payload)->reason == confirm)
                return 0;

            else
                return -1;

            break;

        case set:

            if(((PLMESet *)Message->payload)->reason == confirm)
                return 0;

            else
                return -1;

        case switch_state:

            if(((PLMESwitch *)Message->payload)->reason == confirm)
                return 0;

            else
                return -1;

            break;

        default:
            break;
    }
}



static int executeCommands(struct CommanderPhy *Commander)
{

    int try = 0;
    int ret;
    struct PhyMessageRepo *repo ;
    ServiceMessage *message;


    if(Commander->commands[0])
    {
        for(int i=0; i < Commander->command_index; i++)
        {

            int (*execute_ops)(struct PhyCommand *Command, ServiceMessage *Message) =
                    Commander->commands[i]->ops.execute;

            do
            {
                try++;
                ret = execute_ops(Commander->commands[i], Commander->messages[i]);

                if(ret == -1)
                {
                    return ret;
                }
                else
                {
                    if(ret == MANAGEMENT_COMMAND_RETURN)
                        message = repo->setServiceData(repo, Commander->commands[i]->raw_data_fms);
                    if(ret == DATA_COMMAND_RETURN)
                        message = repo->setServiceData(repo, Commander->commands[i]->raw_data_fds);

                    ret = checkMessage(Commander, message);

                    if(ret =! FAIL)
                        continue;
                    else
                        break;

                }

            }while(try > TRYOUT);

            if(ret != 0)
                return ret;
        }
    }

    return SUCCESS;
}



static int appendCommand(struct CommanderPhy *Commander, struct PhyCommand *Command, ServiceMessage *Message)
{

    Commander->commands[Commander->command_index] = Command;
    Commander->messages[Commander->command_index++] = Message;

}



static int clearCommands(struct CommanderPhy *Commander)
{
    Commander->command_index = 0;

    for(int i=0; i<MAX_COMMAND_SIZE; i++)
    {
        Commander->commands[i] = 0;
        Commander->messages[i] = 0;
    }
}



void initCommanderPhy(struct CommanderPhy *Commander)
{

    Commander->command_index = 0;

    Commander->ops.appendCommand = appendCommand;
    Commander->ops.executeCommands = executeCommands;
    Commander->ops.clearCommands = clearCommands;

    initPhyMessageRepo(&Commander->rx_repo);

}



void deinitCommanderPhy(struct CommanderPhy *Commander)
{

}
