#include <stdio.h>
#include "mac_service.h"
#include "datamessages.h"
#include "mac_frames.h"


int main()
{

    struct MacService mac;

    MacFrameFormat mac_frame;

    char *data = "hello world";

//    mac_frame.header.receiver_address = 0x121212121212;
//    mac_frame.header.transmitter_address = 0x212121212121;

    mac_frame.header.payload_length = 11;
    mac_frame.payload = (uint8_t *)data;
    mac_frame.fcs = 0x20;

    initMacService(&mac);

    mac.ops.sendData(&mac, (uint8_t *)&mac_frame, 11);
//    mac.ops.receiveData(&mac);

    return 0;

}
