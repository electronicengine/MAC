#include <stdio.h>
#include <stdlib.h>
#include "mac_service.h"
#include "datamessages.h"
#include "mac_frames.h"


int main()
{

    struct MacService mac;

    MacFrameFormat mac_frame;
    int selection;


    char *data = "hello world";


//    printf("select 1 for server, 2 for client <<\n");
//    scanf("%d", &selection);

    selection = 1;

    initMacService(&mac, selection);

    mac.ops.sendData(&mac, (uint8_t *)data, 11);

    mac.ops.receiveData(&mac);

    return 0;

}
