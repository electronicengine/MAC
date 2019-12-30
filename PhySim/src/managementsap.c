#include "managementsap.h"
#include "memory.h"
#include "phy.h"

static void deployPLMETrxMessage(struct ManagementSap *Sap)
{

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->type;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->sub_type;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->length & 0xff;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            (Sap->transmitted.header->length << 8) & 0xff;

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.message->payload[0];

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.message->status_or_priorty;

}

static void createPLMETrxMessage(struct ManagementSap *Sap, MassageReason Reason,
                                 SetRxTxStatus Status)
{
    Sap->transmitted.index = 0;

    Sap->transmitted.header->type = phy_management;
    Sap->transmitted.header->sub_type = set_trx;
    Sap->transmitted.header->length = SETTRX_SIZE;

    Sap->transmitted.message->header =
            Sap->transmitted.header;

    Sap->transmitted.trx->reason = Reason;
    Sap->transmitted.message->payload = (uint8_t *)Sap->transmitted.trx;

    Sap->transmitted.message->status_or_priorty = Status;

    deployPLMETrxMessage(Sap);


}

static void deployPLMECCAMessage(struct ManagementSap *Sap)
{

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->type;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->sub_type;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.header->length & 0xff;
    Sap->transmitted.raw[Sap->transmitted.index++] =
            (Sap->transmitted.header->length << 8) & 0xff;

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.message->payload[0];

    Sap->transmitted.raw[Sap->transmitted.index++] =
            Sap->transmitted.message->status_or_priorty;

}


static void createPLMECCAMessage(struct ManagementSap *Sap, MassageReason Reason)
{

    Sap->transmitted.index = 0;

    Sap->transmitted.header->type = phy_management;
    Sap->transmitted.header->sub_type = cca;
    Sap->transmitted.header->length = CCA_SIZE;

    Sap->transmitted.message->header =
            Sap->transmitted.header;

    Sap->transmitted.cca->reason = Reason;
    Sap->transmitted.message->payload =
            (uint8_t *)Sap->transmitted.cca;

    Sap->transmitted.message->status_or_priorty = 0;

    deployPLMECCAMessage(Sap);



}



void initManagementSap(struct ManagementSap *Sap)
{
    Sap->operations.createPLMECCAMessage = createPLMECCAMessage;
    Sap->operations.createPLMETrxMessage = createPLMETrxMessage;
    Sap->operations.generatePLMECCAMessage = generatePLMECCAMessage;
    Sap->operations.generatePLMETrxMessage = generatePLMETrxMessage;

    Sap->transmitted.header = malloc(sizeof(ServiceMessageHeader));
    Sap->transmitted.message = malloc(sizeof(ServiceMessage));
    Sap->transmitted.message->header = Sap->transmitted.header;

    Sap->received.header = malloc(sizeof(ServiceMessageHeader));
    Sap->received.message = malloc(sizeof(ServiceMessage));
    Sap->received.message->header = Sap->received.header;

    Sap->transmitted.cca = malloc(sizeof(PLMECCA));
    Sap->transmitted.get = malloc(sizeof(PLMEGet));
    Sap->transmitted.set = malloc(sizeof(PLMESet));
    Sap->transmitted.trx = malloc(sizeof(PLMESetTRX));
    Sap->transmitted.sw = malloc(sizeof(PLMESwitch));

    Sap->received.cca = malloc(sizeof(PLMECCA));
    Sap->received.get = malloc(sizeof(PLMEGet));
    Sap->received.set = malloc(sizeof(PLMESet));
    Sap->received.trx = malloc(sizeof(PLMESetTRX));
    Sap->received.sw = malloc(sizeof(PLMESwitch));

    Sap->received.index = 0;
    Sap->transmitted.index = 0;

}

void deinitManagementSap(struct ManagementSap *Sap)
{
    free(Sap->transmitted.header);
    free(Sap->transmitted.cca);
    free(Sap->transmitted.get);
    free(Sap->transmitted.set);
    free(Sap->transmitted.trx);
    free(Sap->transmitted.sw);

    free(Sap->received.message);

    free(Sap->received.header);
    free(Sap->received.cca);
    free(Sap->received.get);
    free(Sap->received.set);
    free(Sap->received.trx);
    free(Sap->received.sw);

    free(Sap->received.message);

}



static void generatePLMECCAMessage(struct ManagementSap *Sap, uint8_t *Data)
{
    Sap->received.index = 0;

    Sap->received.header->type = Data[Sap->received.index++];
    Sap->received.header->sub_type = Data[Sap->received.index++];
    Sap->received.header->length = Data[Sap->received.index++] & 0xff;
    Sap->received.header->length |= (Data[Sap->received.index++] << 8) & 0xff;

    Sap->received.message->header = Sap->received.header;

    Sap->received.cca->reason = Data[Sap->received.index++];
    Sap->received.message->payload = (uint8_t *) Sap->received.cca;

    Sap->received.message->status_or_priorty = Data[Sap->received.index++];
}



static void generatePLMETrxMessage(struct ManagementSap *Sap, uint8_t *Data)
{
    Sap->received.index = 0;

    Sap->received.header->type = Data[Sap->received.index++];
    Sap->received.header->sub_type = Data[Sap->received.index++];
    Sap->received.header->length = Data[Sap->received.index++] & 0xff;
    Sap->received.header->length |= (Data[Sap->received.index++] << 8) & 0xff;

    Sap->received.message->header = Sap->received.header;

    Sap->received.trx->reason = Data[Sap->received.index++];
    Sap->received.message->payload = (uint8_t *) Sap->received.trx;

    Sap->received.message->status_or_priorty = Data[Sap->received.index++];
}
