#include <mac.h>




int OpenDevice()
{


}



int closeDevice()
{

    printf("closing Device\n");

}



void receiveData(struct MacService *Mac, uint8_t *Data, uint8_t Length)
{
    struct ChainBase *chain = &Mac->Chain;

    chain->operations.addHandle(chain, &Mac->cca.base);
    chain->operations.addHandle(chain, &Mac->RxOn.base);
    chain->operations.addHandle(chain, &Mac->TrxOff.base);

    chain->operations.handle(chain, chain->next_chain, Data, Length);

}



void sendData(struct MacService *Mac, uint8_t *Data, uint16_t Length)
{

    struct ChainBase *chain = &Mac->Chain;

    chain->operations.addHandle(chain, &Mac->RxOn.base);
    chain->operations.addHandle(chain, &Mac->cca.base);
    chain->operations.addHandle(chain, &Mac->TxOn.base);
    chain->operations.addHandle(chain, &Mac->Transmitter.base);
    chain->operations.addHandle(chain, &Mac->TrxOff.base);

    chain->operations.handle(chain,chain->next_chain, Data, Length);

}



void initMac(struct MacService *Mac)
{

    struct Subject *subject = &Mac->unix_sock.subject;

    Mac->operations.sendData = sendData;
    Mac->operations.receiveData = receiveData;

    initSocket(&Mac->unix_sock);
    initChainBase(&Mac->Chain, &Mac->unix_sock);
    initRxOn(&Mac->RxOn, &Mac->unix_sock);
    initCCA(&Mac->cca, &Mac->unix_sock);
    initTxOn(&Mac->TxOn, &Mac->unix_sock);
    initDataTransmitter(&Mac->Transmitter, &Mac->unix_sock);
    initTrxOff(&Mac->TrxOff, &Mac->unix_sock);

    subject->operations.registerObserver(subject, &Mac->RxOn.observer);
    subject->operations.registerObserver(subject, &Mac->cca.observer);
    subject->operations.registerObserver(subject, &Mac->TxOn.observer);
    subject->operations.registerObserver(subject, &Mac->Transmitter.observer);
    subject->operations.registerObserver(subject, &Mac->TrxOff.observer);

}



void destroyMessage(ServiceMessage *Message)
{

    free(Message->payload);
    free(Message);

}


