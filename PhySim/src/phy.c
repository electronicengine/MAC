#include <phy.h>



void initPhy(struct PhyService *Phy)
{

    struct Subject *subject = &Phy->unix_socket.subject;

    initSocket(&Phy->unix_socket);

    initCCA(&Phy->cca);
    initDataTransmitter(&Phy->transmitter);
    initTrxState(&Phy->trx);
    subject->operations.registerObserver(subject, &Phy->trx.observer);
    subject->operations.registerObserver(subject, &Phy->cca.observer);
    subject->operations.registerObserver(subject, &Phy->transmitter.observer);

}


