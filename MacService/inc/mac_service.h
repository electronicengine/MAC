#ifndef __mac_service_H
#define __mac_service_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "commander_mac.h"
#include "mac_management_sap.h"
#include "mac_data_sap.h"
#include "datamessages.h"
#include "managementmessages.h"
#include "management_frames.h"
#include "mac_message_repo.h"



#define SERVER_OWPAN_ADDR   0x121212121212
#define CLIENT_OWPAN_ADDR   0x212121212121

struct MacService;


struct MacServiceOperations
{

    int (*connectNetwork)(struct MacService *Service, uint64_t OwpanAddr);
    int (*createNetwork)(struct MacService *Service, uint64_t OwpanAddr);
    int (*sendData)(struct MacService *Service, uint8_t *Data, uint16_t Length);
    int (*receiveData)(struct MacService *Service);

};



struct MacService
{
    struct MacServiceOperations ops;
    struct CommanderMac         commander_mac;
    struct MacManagementSap     mac_management_sap;
    struct MacDataSap           mac_data_sap;

    struct MacMessageRepo   mac_message_repo;



};



void initMacService(struct MacService *Service, int Selection);
void deinitMacService(struct MacService *Service);




#endif
