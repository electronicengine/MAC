#include "cca.h"
#include "phy_message_repo.h"
#include "unixsocket.h"




static void confirmRequest(struct UnixSocket *Socket, ServiceMessage *Message)
{
    uint8_t *transmit_data;
    struct PhyMessageRepo *repo = &Socket->phy_repo;
    int data_index = 0;

    printf("confirmingRequest CCA\n");

    ((PLMECCA *)Message->payload)->reason = confirm;

    transmit_data = repo->convertMessagetoRaw(repo, Message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);
}



static void spiDataUpdate(struct UnixSocket *Socket, ServiceMessage *Message)
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

    cca->operations.spiDataUpdate = spiDataUpdate;


    initObserver(&cca->observer);
    cca->observer.operation.update = spiDataUpdate;

}



