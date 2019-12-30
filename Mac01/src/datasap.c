#include "datasap.h"


static void generateDataHeader(struct DataSap *Sap, uint8_t *Data)
{

    Sap->received.index = 0;

    Sap->received.header->type = Data[Sap->received.index++];
    Sap->received.header->sub_type = Data[Sap->received.index++];
    Sap->received.header->length = Data[Sap->received.index++];
    Sap->received.header->length |= (Data[Sap->received.index++] << 8);

}



static void generateDataPayload(struct DataSap *Sap, uint8_t *Data)
{

    Sap->received.data->reason = Data[Sap->received.index++];

    Sap->received.data->payload = &Data[Sap->received.index];
    Sap->received.index += Sap->received.header->length;

    Sap->received.data->link_quality = Data[Sap->received.index++];

}



void generatePDMassage(struct DataSap *Sap, uint8_t *Data, uint16_t Length)
{

    generateDataHeader(Sap, Data);

    generateDataPayload(Sap, Data);

    Sap->received.message->status_or_priorty = Data[Sap->received.index++];


}



static void createPDMessage(struct DataSap *Sap, MassageReason Reason, uint8_t *Data, uint16_t Length)
{

    createDataHeader(Sap, Length);

    createDataPayload(Sap, Reason, Data, Length);

    Sap->transmit.message->status_or_priorty = 0;
    Sap->transmit.raw[Sap->transmit.index++] =
            Sap->transmit.message->status_or_priorty;


}



void initDataSap(struct DataSap *Sap)
{

    Sap->operations.createPDMessage = createPDMessage;
    Sap->operations.generatePDMassage = generatePDMassage;

    Sap->transmit.header = malloc(sizeof(ServiceMessageHeader));
    Sap->transmit.message = malloc(sizeof(ServiceMessage));
    Sap->transmit.data = malloc(sizeof(PhyData));

    Sap->received.header = malloc(sizeof(ServiceMessageHeader));
    Sap->received.message = malloc(sizeof(ServiceMessage));
    Sap->received.data = malloc(sizeof(PhyData));

    Sap->transmit.message->header = Sap->transmit.header;
    Sap->transmit.message->payload = (uint8_t *)Sap->transmit.data;

    Sap->received.message->header = Sap->received.header;
    Sap->received.message->payload = (uint8_t *)Sap->received.data;

    Sap->transmit.index = 0;
    Sap->received.index = 0;

}



void deinitDataSap(struct DataSap *Sap)
{

    free(Sap->transmit.data);
    free(Sap->transmit.header);
    free(Sap->transmit.message);

    free(Sap->received.data);
    free(Sap->received.header);
    free(Sap->received.message);

}



static void createDataPayload(struct DataSap *Sap, MassageReason Reason, uint8_t *PayloadData, uint16_t Length)
{

    Sap->transmit.data->reason = Reason;
    Sap->transmit.data->payload = PayloadData;
    Sap->transmit.data->link_quality = 0;

    Sap->transmit.raw[Sap->transmit.index++] =
            Sap->transmit.data->reason;

    memcpy(&Sap->transmit.raw[Sap->transmit.index],
            Sap->transmit.data->payload, Length);

    Sap->transmit.index += Length;

    Sap->transmit.raw[Sap->transmit.index++] =
            Sap->transmit.data->link_quality;

}



static void createDataHeader(struct DataSap *Sap, uint16_t DataLength)
{

    Sap->transmit.index = 0;

    Sap->transmit.header->type = phy_data;
    Sap->transmit.header->sub_type = transmit;
    Sap->transmit.header->length = DataLength;

    Sap->transmit.raw[Sap->transmit.index++] = Sap->transmit.header->type;
    Sap->transmit.raw[Sap->transmit.index++] = Sap->transmit.header->sub_type;
    Sap->transmit.raw[Sap->transmit.index++] = Sap->transmit.header->length & 0xff;
    Sap->transmit.raw[Sap->transmit.index++] = ((Sap->transmit.header->length >> 8) & 0xff);

}
