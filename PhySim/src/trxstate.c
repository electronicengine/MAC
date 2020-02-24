#include "trxstate.h"
#include "phy_message_repo.h"
#include "unixsocket.h"



static void confirmRequest(struct UnixSocket *Socket, ServiceMessage *PhyMessage)
{

    uint8_t *transmit_data;
    struct PhyMessageRepo *repo = &Socket->phy_repo;
    int data_index = 0;

    if(PhyMessage->status_or_priorty== rx_on)
        printf("confirmingRequest settrx_rx_on\n");
    if(PhyMessage->status_or_priorty == tx_on)
        printf("confirmingRequest settrx_tx_on\n");
    if(PhyMessage->status_or_priorty == trx_off)
        printf("confirmingRequest settrx_trx_off\n");

    ((PLMECCA *)PhyMessage->payload)->reason = confirm;

    printf("length: %d\n", PhyMessage->header.length);

    transmit_data = repo->convertMessagetoRaw(repo, PhyMessage, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);
}



static void spiDataUpdate(struct UnixSocket *Socket, ServiceMessage *Message)
{

    if(Message->header.type == phy_management && Message->header.sub_type == set_trx)
    {

        switch (((PLMESetTRX *)Message->payload)->reason)
        {

            case request:

                confirmRequest(Socket, Message);

                break;

            case confirm:


                break;

            case indication:

                break;


            case response:

                break;

            default:
                break;
        }

    }

}




void initTrxState(struct TRXState *Trx)
{

    initObserver(&Trx->observer);

    Trx->observer.operation.update = spiDataUpdate;

}


