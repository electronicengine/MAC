#include <datatransmitter.h>
#include "phy.h"


static void confirmDataTransmitRequest(struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;
    ServiceMessage *confirm_message;

    printf("confirm dataTransmitterReqest\n");

    confirm_message->header.type = phy_data;
    confirm_message->header.sub_type = transmit;
    confirm_message->header.length = 1;
    confirm_message->payload = 0;
    confirm_message->status_or_priorty = 0;

    transmit_data = phy_repo->convertMessagetoRaw(phy_repo, confirm_message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void confirmDataReceiveRequest(struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    int data_index = 0;
    struct PhyMessageRepo *phy_repo = &Socket->phy_repo;

    ServiceMessage *indication_message;


    printf("confirm dataReceiveRequest\n");

    ((PhyData *)PhyMessage->payload)->reason = confirm;
    transmit_data = phy_repo->convertMessagetoRaw(phy_repo, PhyMessage, &data_index);
    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void spiDataUpdate(struct UnixSocket *Socket, ServiceMessage *Message)
{

    if(Message->header.type == phy_data && (Message->header.sub_type == transmit || Message->header.sub_type == receive))
    {


        switch (((PhyData *)Message->payload)->reason)
        {

            case request:

                if(Message->header.sub_type == transmit)
                    confirmDataTransmitRequest(Socket, Message);
                if(Message->header.sub_type == receive)
                    confirmDataReceiveRequest(Socket, Message);

                break;

            case confirm:


                break;

            case indication:


                break;

            default:
                break;
        }
    }
}






void initDataTransmitter(struct DataTransmitter *Transmitter)
{

    Transmitter->operations.spiDataUpdate = spiDataUpdate;


    initObserver(&Transmitter->observer);
    Transmitter->observer.operation.update = spiDataUpdate;


}



