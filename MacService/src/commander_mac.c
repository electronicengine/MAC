#include "commander_mac.h"


static int executeCommands(struct CommanderMac *Commander);
static int appendCommand(struct CommanderMac *Commander, struct MacCommand *Command, ServiceMessage *Message);
static int clearCommands(struct CommanderMac *Commander);



static int executeCommands(struct CommanderMac *Commander)
{
    int ret;
    ServiceMessage *mac_message = Commander->indication_message;

    printf("commander mac execute\n");

    if(Commander->commands[0])
    {

        for (int i=0; i < Commander->command_index; i++)
        {

            int (*execute_ops)(struct MacCommand *Command, ServiceMessage *Message) =
                    Commander->commands[i]->ops.execute;

                ret = execute_ops(Commander->commands[i], Commander->messages[i]);

                if(ret == FAIL)
                    return ret;

                else
                {
                    if(ret == DATA_RECEIVE_RETURN)
                    {

                        ServiceMessage message;

                        printf("MAC DATA_RECEIVE_RETURN\n");

                        mac_message = Commander->rx_repo.setServiceMessage(&Commander->rx_repo,
                                                                           Commander->commands[i]->mac_indication_data);

                        Commander->indication_message = mac_message;

                    }
                    else if(ret == DATA_COMMAND_RETURN)
                    {
                        printf("MAC DATA_COMMAND_RETURN\n");
                        // to do something

                    }
                    else if(ret == MANAGEMENT_COMMAND_RETURN)
                    {
                        printf("MAC MANAGEMENT_COMMAND_RETURN\n");
                        // to do something

                    }
                    else
                        return FAIL;

                }
        }
    }

    return SUCCESS;
}



static int appendCommand(struct CommanderMac *Commander, struct MacCommand *Command, ServiceMessage *Message)
{

    printf("commander mac append message %d\n", Commander->command_index);

    Commander->commands[Commander->command_index] = Command;
    Commander->messages[Commander->command_index] = Message;
    Commander->command_index++;

}



static int clearCommands(struct CommanderMac *Commander)
{

    printf("commander mac clear commands\n");
    for(int i=0; i<Commander->command_index; i++)
    {
        Commander->commands[i] = 0;
        Commander->messages[i] = 0;
    }

    Commander->command_index = 0;

}



void initCommanderMac(struct CommanderMac *Commander)
{

    Commander->command_index = 0;

    Commander->ops.appendCommand = appendCommand;
    Commander->ops.executeCommands = executeCommands;
    Commander->ops.clearCommands = clearCommands;

    initMacMessageRepo(&Commander->rx_repo);

}



void deinitCommanderMac(struct CommanderMac *Commander)
{

}
