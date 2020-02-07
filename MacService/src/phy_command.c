#include "phy_command.h"



void initPhyCommand(struct PhyCommand *Command)
{

    initSocket(&Command->sock);

}



void deinitphyCommand(struct PhyCommand *Command)
{
    deinitSocket(&Command->sock);
}
