#ifndef __datasap_H
#define __datasap_H



#include <inttypes.h>
#include <memory.h>
#include <stdio.h>
#include <stdlib.h>

#include "managementmessages.h"
#include "datamessages.h"

#define MAX_TRANSMITED_DATA             1600
#define SERVICE_MESSAGE_HEADER_SIZE     4
#define PAYLOAD_LINK_QUALITY_SIZE       1
#define PAYLOAD_REASON_SIZE             1
#define SERVICE_MESSAGE_STATUS_SIZE     1


struct DataSap;



struct DataSapOperations
{

    void (*createPDMessage)(struct DataSap *Sap, MassageReason Reason, uint8_t *Data, uint16_t Length);
    void (*generatePDMassage)(struct DataSap *Sap, uint8_t *Data, uint16_t Length);

};

typedef struct
{
    ServiceMessage *message;
    ServiceMessageHeader *header;
    PhyData *data;

    uint8_t raw[MAX_TRANSMITED_DATA];
    uint16_t index;

}DataBuffer;


struct DataSap
{

    struct DataSapOperations operations;

    DataBuffer received;
    DataBuffer transmit;

};



static void createPDMessage(struct DataSap *Sap, MassageReason Reason, uint8_t *Data, uint16_t Length);
static void generatePDMassage(struct DataSap *Sap, uint8_t *Data, uint16_t Length);

static void generateDataHeader(struct DataSap *Sap, uint8_t *Data);
static void generateDataPayload(struct DataSap *Sap, uint8_t *Data);

static void createDataPayload(struct DataSap *Sap, MassageReason Reason, uint8_t *PayloadData, uint16_t Length);
static void createDataHeader(struct DataSap *Sap, uint16_t DataLength);


void initDataSap(struct DataSap *Sap);
void deinitDataSap(struct DataSap *Sap);

#endif
