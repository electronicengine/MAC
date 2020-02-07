#include <stdio.h>
#include "mac_service.h"
#include "datamessages.h"

int main()
{

    struct MacService mac;

    char *data = "hello world";
    uint16_t size = 11;

    initMacService(&mac);

    mac.ops.sendData(&mac, data, 11);




    return 0;

}
