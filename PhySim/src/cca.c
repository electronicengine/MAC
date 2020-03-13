#include "cca.h"
#include "phy_message_repo.h"
#include "mac_socket.h"



static uint8_t *convertMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PLMECCA *plme_cca = (PLMECCA *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    raw_data[(*Index)++] = plme_cca->reason;

    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



static void confirmRequest(struct MacSocket *Socket, ServiceMessage *Message)
{
    uint8_t *transmit_data;
    struct PhyMessageRepo *repo = &Socket->phy_repo;
    int data_index = 0;

    printf("confirmingRequest CCA\n");

    ((PLMECCA *)Message->payload)->reason = confirm;

    transmit_data = convertMessagetoRaw(repo, Message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);
}



static void updateSocket(struct Observer *Observer, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData)
{
    if(Message->header.type == phy_management && Message->header.sub_type == cca)
    {

        switch (((PLMECCA *)Message->payload)->reason)
        {

            case request:

                confirmRequest(Socket, Message);

                break;

            case confirm:

//                respondRequest(Socket);


                break;

            case indication:


                break;

            default:
                break;
        }

    }



}






void initCCA(struct CCA *cca)
{

    cca->operations.updateSocket = updateSocket;


    initObserver(&cca->observer);
    cca->observer.operation.update = updateSocket;

}



