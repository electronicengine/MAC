#include "commander_phy.h"
#include "mac_frames.h"


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
            {
                printf( "received confirm PLMECCA\n");
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

        case set_trx:

            if(((PLMESetTRX *)Message->payload)->reason == confirm)
            {
                printf( "received confirm PLMESetTrx\n");
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

        case transmit:


            if(((PhyData *)Message->payload)->reason == confirm)
            {
                printf( "received confirm PhyData Transmit\n");

                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

        case receive:

            if(((PhyData *)Message->payload)->reason == indication)
            {
                printf( "received confirm PhyData Received Indication\n");
                Commander->phy_indication_data = Message;
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

        case get:

            if(((PLMEGet *)Message->payload)->reason == confirm)
            {
                printf( "received confirm PLMEGet\n");
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

            break;

        case set:

            if(((PLMESet *)Message->payload)->reason == confirm)
            {
                printf( "received confirm PLMESet\n");
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESet Failed\n");
                return FAIL;
            }

        case switch_state:

            if(((PLMESwitch *)Message->payload)->reason == confirm)
            {
                printf( "received confirm PLMESwitch\n");
                return SUCCESS;
            }
            else
            {
                printf( "received confirm PLMESwitch Failed\n");
                return FAIL;
            }

            break;

        default:
            break;
    }
}



static int executeCommands(struct CommanderPhy *Commander)
{

    int try_num = 0;
    int ret;
    struct PhyMessageRepo *repo = &Commander->rx_repo;
    ServiceMessage *message;


    if(Commander->commands[0])
    {
        for(int i=0; i < Commander->command_index; i++)
        {


            int (*execute_ops)(struct PhyCommand *Command, ServiceMessage *Message) =
                    Commander->commands[i]->ops.execute;

            do
            {
                printf("commander phy execute command %d\n", i);

                try_num++;
                ret = execute_ops(Commander->commands[i], Commander->messages[i]);

                if(ret == FAIL)
                {
                    return ret;
                }
                else
                {


                    if(ret == MANAGEMENT_COMMAND_RETURN)
                    {
                        message = repo->setServiceData(repo, Commander->commands[i]->raw_data_fms);
                        printf("raw: ");
                        for(int k = 0; k < 8; k++)
                            printf("%02X-", Commander->commands[i]->raw_data_fms[k]);
                        printf("\n");
                    }
                    if(ret == DATA_COMMAND_RETURN || ret == DATA_RECEIVE_RETURN)
                    {
                        message = repo->setServiceData(repo, Commander->commands[i]->raw_data_fds);
                        printf("raw: ");
                        for(int k = 0; k < 8; k++)
                            printf("%02X-", Commander->commands[i]->raw_data_fds[k]);
                        printf("\n");
                    }

                    ret = checkMessage(Commander, message);

                    if(ret == FAIL)
                    {
                        printf("reattempting...\n");
                        continue;
                    }
                    else
                    {
                        break;
                    }

                }

            }while(try_num < TRYOUT);

            if(ret == FAIL)
            {
                printf("failed!\n");
                break;
            }

        }
    }

    return ret;
}



static int appendCommand(struct CommanderPhy *Commander, struct PhyCommand *Command, ServiceMessage *Message)
{

    printf("commander phy append command %d\n", Commander->command_index);
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
