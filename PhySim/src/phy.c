#include <phy.h>



int OpenDevice()
{


}



int closeDevice()
{

    printf("closing Device\n");
    
}



static void receiveData(struct PhyService *Mac, uint8_t *Data, uint8_t Length)
{
    struct ChainBase *chain = &Mac->Chain;

    chain->operations.addHandle(chain, &Mac->cca.base);
    chain->operations.addHandle(chain, &Mac->RxOn.base);
    chain->operations.addHandle(chain, &Mac->TrxOff.base);

    chain->operations.handle(chain, Data, Length);

}



static void sendData(struct PhyService *Phy, uint8_t *Data, uint8_t Length)
{

    sleep(1);

    printf("phy send\n");
    Phy->unix_socket.operations.setData(&Phy->unix_socket, Data, Length);

//    chainAdd(&Mac->Chain, &Mac->RxOn.base);
//    chainAdd(&Mac->Chain, &Mac->cca.base);
//    chainAdd(&Mac->Chain, &Mac->TxOn.base);
//    chainAdd(&Mac->Chain, &Mac->Transmitter.base);
//    chainAdd(&Mac->Chain, &Mac->TrxOff.base);

//    chainHandle(&Mac->Chain, Data, Length);

}



void initPhy(struct PhyService *Phy)
{

    struct Subject *subject = &Phy->unix_socket.subject;

    Phy->operations.sendData = sendData;
    Phy->operations.receiveData = receiveData;

    initSocket(&Phy->unix_socket);

    initChainBase(&Phy->Chain, &Phy->unix_socket);
    initRxOn(&Phy->RxOn, &Phy->unix_socket);
    initCCA(&Phy->cca, &Phy->unix_socket);
    initTxOn(&Phy->TxOn, &Phy->unix_socket);
    initDataTransmitter(&Phy->Transmitter, &Phy->unix_socket);
    initTrxOff(&Phy->TrxOff, &Phy->unix_socket);

    subject->operations.registerObserver(subject, &Phy->RxOn.observer);
    subject->operations.registerObserver(subject, &Phy->cca.observer);
//    subject->operations.registerObserver(subject, &Phy->TxOn.observer);
    subject->operations.registerObserver(subject, &Phy->Transmitter.observer);
//    subject->operations.registerObserver(subject, &Phy->TrxOff.observer);

}



void destroyMessage(ServiceMessage *Message)
{

    free(Message->payload);
    free(Message);

}
