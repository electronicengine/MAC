#include "setphy.h"
#include "phy_message_repo.h"
#include "mac_socket.h"



static int startOwpan(struct SetPhy *Set, uint8_t OwpanCoord)
{
    printf("Owpan Starting... Owpa Coord: %d\n", OwpanCoord);

    if(OwpanCoord == 1)
        Set->observer.wireless_socket.ops.openServerPort(&Set->observer.wireless_socket);
    else if(OwpanCoord == 0)
        Set->observer.wireless_socket.ops.openClientPort(&Set->observer.wireless_socket);

}



static uint8_t *convertMessagetoRaw(struct PhyMessageRepo *Repo, ServiceMessage *PhyMessage, int *Index)
{

    uint8_t *raw_data = Repo->getRawData(Repo);
    PLMESet *plme_set = (PLMESet *)PhyMessage->payload;

    raw_data[(*Index)++] = PhyMessage->header.type;
    raw_data[(*Index)++] = PhyMessage->header.sub_type;
    raw_data[(*Index)++] = PhyMessage->header.length & 0xff;
    raw_data[(*Index)++] = (PhyMessage->header.length >> 8) & 0xff;

    raw_data[(*Index)++] = plme_set->reason;
    raw_data[(*Index)++] = PhyMessage->status_or_priorty;

    return raw_data;

}



static void confirmRequest(struct SetPhy *Set, struct MacSocket *Socket, ServiceMessage *Message)
{

    uint8_t *transmit_data;
    struct PhyMessageRepo *repo = &Socket->phy_repo;
    int data_index = 0;

    printf("confirmingRequest SetPhy\n");

    ((PLMESet *)Message->payload)->reason = confirm;

    if(((PLMESet *)Message->payload)->pib_attribute == cca_mode)
        printf("cca_mode %d\n",((PLMESet *)Message->payload)->pib_attribute_value);
    if(((PLMESet *)Message->payload)->pib_attribute == owpan_coordinator)
        startOwpan(Set, ((PLMESet *)Message->payload)->pib_attribute_value);
    if(((PLMESet *)Message->payload)->pib_attribute == beacon_order)
        printf("beacon_order %d\n", ((PLMESet *)Message->payload)->pib_attribute_value);
    if(((PLMESet *)Message->payload)->pib_attribute == superframe_order)
        printf("superframe_order %d\n", ((PLMESet *)Message->payload)->pib_attribute_value);

    transmit_data = convertMessagetoRaw(repo, Message, &data_index);

    Socket->operations.setData(Socket, transmit_data, data_index);

}



static void updateSocket(struct Observer *Obs, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData)
{

    struct SetPhy *set_phy = container_of(Obs, typeof(*set_phy), observer);


    if(Message->header.type == phy_management && Message->header.sub_type == set)
    {

        switch (((PLMESet *)Message->payload)->reason)
        {

            case request:

                confirmRequest(set_phy, Socket, Message);

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

    initObserver(&Setphy->observer);
    Setphy->observer.operation.update = updateSocket;

}



