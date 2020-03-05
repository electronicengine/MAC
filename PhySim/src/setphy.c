#include "setphy.h"
#include "phy_message_repo.h"
#include "macsocket.h"



static uint8_t *convertMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PLMESet *plme_cca = (PLMESet *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    raw_data[(*Index)++] = plme_cca->reason;

    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



static void confirmRequest(struct UnixSocket *Socket, ServiceMessage *Message)
{
    uint8_t *transmit_data;
    struct PhyMessageRepo *repo = &Socket->phy_repo;
    int data_index = 0;

    printf("confirmingRequest SetPhy\n");

    ((PLMESet *)Message->payload)->reason = confirm;

    transmit_data = convertMessagetoRaw(repo, Message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);
}



static void spiDataUpdate(struct Observer *Observer, struct UnixSocket *Socket, ServiceMessage *Message)
{
    if(Message->header.type == phy_management && Message->header.sub_type == set)
    {

        switch (((PLMESet *)Message->payload)->reason)
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






void initSetPhy(struct SetPhy *Setphy)
{

    Setphy->operations.spiDataUpdate = spiDataUpdate;


    initObserver(&Setphy->observer);
    Setphy->observer.operation.update = spiDataUpdate;

}



