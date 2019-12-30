#ifndef __managementsap_H
#define __managementsap_H


#include <inttypes.h>
#include "macframe.h"
#include "datamessages.h"
#include "managementmessages.h"

#define MAX_DATA_SIZE   1600
#define SETTRX_SIZE     1
#define CCA_SIZE        1




typedef struct
{
    ServiceMessage          *message;
    ServiceMessageHeader    *header;

    PLMECCA     *cca;
    PLMEGet     *get;
    PLMESet     *set;
    PLMESetTRX  *trx;
    PLMESwitch  *sw;

    uint8_t raw[MAX_DATA_SIZE];
    uint16_t index;


}ManagementBuffer;


struct ManagementSap;

struct ManagementSapOperations
{

    void (*createPLMECCAMessage)(struct ManagementSap *Sap, MassageReason Reasons);
    void (*generatePLMECCAMessage)(struct ManagementSap *Sap, uint8_t *Data);


    void (*createPLMETrxMessage)(struct ManagementSap *Sap, MassageReason Reason,
                                 SetRxTxStatus Status);
    void (*generatePLMETrxMessage)(struct ManagementSap *Sap, uint8_t *Data);

};



struct ManagementSap
{

    struct ManagementSapOperations operations;

    ManagementBuffer transmitted;
    ManagementBuffer received;

};



void initManagementSap(struct ManagementSap *Sap);
void deinitManagementSap(struct ManagementSap *Sap);

static void createPLMETrxMessage(struct ManagementSap *Sap,MassageReason Reason,
                                 SetRxTxStatus Status);
static void createPLMECCAMessage(struct ManagementSap *Sap, MassageReason Reason);
static void generatePLMECCAMessage(struct ManagementSap *Sap, uint8_t *Data);
static void generatePLMETrxMessage(struct ManagementSap *Sap, uint8_t *Data);


#endif
