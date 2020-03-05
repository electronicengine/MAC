#ifndef __mac_message_repo_H
#define __mac_message_repo_H


#include "messagetypes.h"
#include "datamessages.h"
#include "managementmessages.h"
#include "macframe.h"

#include <memory.h>

#define MAX_COMMAND_SIZE   32



struct MacMessageIndex
{
    int service_message;
    int mcsp_data;
    int mlme_associate;
    int mlme_disassociate;
    int mlme_get;
    int mlme_scan;
    int mlme_start;
    int mlme_stop;
    int raw_data;


};

struct MacMessageRepo
{
    struct MacMessageIndex index;

    ServiceMessage service_message[AVAILABLE_MESSAGE];

    MCSPData mcsp_data[AVAILABLE_MESSAGE];
    MLMEAssociate mlme_associate[AVAILABLE_MESSAGE];
    MLMEDisassociate mlme_disassociate[AVAILABLE_MESSAGE];
    MLMEGet mlme_get[AVAILABLE_MESSAGE];
    MLMEScan mlme_scan[AVAILABLE_MESSAGE];
    MLMEStart mlme_start[AVAILABLE_MESSAGE];
    MLMEStop mlme_stop[AVAILABLE_MESSAGE];
    uint8_t raw_data[AVAILABLE_MESSAGE][MAX_MESSAGE_SIZE];

    ServiceMessage *(*getServiceMessage)(struct MacMessageRepo *);

    MCSPData *(*getMcspData)(struct MacMessageRepo *);

    MLMEAssociate *(*getMlmeAssociate)(struct MacMessageRepo *);
    MLMEDisassociate *(*getMlmeDisassociate)(struct MacMessageRepo *);
    MLMEGet *(*getMlmeGet)(struct MacMessageRepo *);
    MLMEScan *(*getMlmeScan)(struct MacMessageRepo *);
    MLMEStart *(*getMlmeStart)(struct MacMessageRepo *);
    MLMEStop *(*getMlmeStop)(struct MacMessageRepo *);

    ServiceMessage *(*setServiceMessage)(struct MacMessageRepo *, ServiceMessage *);



};



void initMacMessageRepo(struct MacMessageRepo *Repo);
void deinitMacMessageRepo(struct MacMessageRepo *Repo);

#endif
