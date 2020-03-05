#include "trxstate.h"
#include "phy_message_repo.h"
#include "macsocket.h"


static uint8_t *convertMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PLMESetTRX *plme_settrx = (PLMESetTRX *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    raw_data[(*Index)++] = plme_settrx->reason;

    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



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

    ((PLMESetTRX *)PhyMessage->payload)->reason = confirm;

    transmit_data = convertMessagetoRaw(repo, PhyMessage, &data_index);

    printf("answer raw: ");
    for(int i = 0; i < 7; i++)
        printf("%02X-", transmit_data[i]);

    printf("\n");

    Socket->operations.setData(Socket, transmit_data, data_index);
}



static void spiDataUpdate(struct Observer *Observer, struct UnixSocket *Socket, ServiceMessage *Message)
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


