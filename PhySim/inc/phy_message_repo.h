#ifndef __phy_message_repo_H
#define __phy_message_repo_H


#include "messagetypes.h"
#include "datamessages.h"
#include "managementmessages.h"
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>


struct PhyMessageRepo
{
    ServiceMessage  service_message[AVAILABLE_MESSAGE];

    PhyData pd_data[AVAILABLE_MESSAGE];

    PLMESetTRX plme_settrx[AVAILABLE_MESSAGE];
    PLMEGet plme_get[AVAILABLE_MESSAGE];
    PLMESet plme_set[AVAILABLE_MESSAGE];
    PLMESwitch plme_switch[AVAILABLE_MESSAGE];
    PLMECCA plme_cca[AVAILABLE_MESSAGE];
    uint8_t raw_data[AVAILABLE_MESSAGE][MAX_MESSAGE_SIZE];

    ServiceMessage *(*getServiceMessage)(struct PhyMessageRepo *);
    PhyData *(*getPhyData)(struct PhyMessageRepo *);

    PLMESetTRX *(*getPlmeSetTrx)(struct PhyMessageRepo *);
    PLMECCA *(*getPlmeCca)(struct PhyMessageRepo *);
    PLMEGet *(*getPlmeGet)(struct PhyMessageRepo *);
    PLMESet *(*getPlmeSet)(struct PhyMessageRepo *);
    PLMESwitch *(*getPlmeSwitch)(struct PhyMessageRepo *);
    uint8_t *(*getRawData)(struct PhyMessageRepo *);

    uint8_t *(*convertMessagetoRaw)(struct PhyMessageRepo *, ServiceMessage *PhyMessage, int *Index);
    ServiceMessage *(*setServiceData)(struct PhyMessageRepo *, uint8_t *);

};


void initPhyMessageRepo(struct PhyMessageRepo *Repo);
void deinitPhyMessageRepo(struct PhyMessageRepo *Repo);



#endif
