#include <phy.h>



void initPhy(struct PhyService *Phy)
{

    struct Subject *subject = &Phy->mac_socket.subject;

    initMacSocket(&Phy->mac_socket);
    initWirelessSocket(&Phy->wireless_socket);

    initCCA(&Phy->cca);
    initDataTransmitter(&Phy->transmitter, &Phy->wireless_socket);
    initTrxState(&Phy->trx);
    initSetPhy(&Phy->setphy, &Phy->wireless_socket);

    subject->operations.registerObserver(subject, &Phy->trx.observer);
    subject->operations.registerObserver(subject, &Phy->cca.observer);
    subject->operations.registerObserver(subject, &Phy->transmitter.observer);
    subject->operations.registerObserver(subject, &Phy->setphy.observer);

}


