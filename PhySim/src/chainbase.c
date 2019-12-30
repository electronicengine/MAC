#include "chainbase.h"
#include <inttypes.h>
#include "memory.h"



static void addHandle(struct ChainBase *Base, struct ChainBase *Next)
{

    if(Base)
    {
        if (Base->next_chain)
            addHandle(Base->next_chain, Next);
        else
            Base->next_chain = Next;
    }

}



static int handle(struct ChainBase *Base, uint8_t *Data, uint8_t Length)
{

    printf("Chain Base\n");

    if (Base->next_chain)
        Base->operations.handle(Base->next_chain, Data, Length);

    Base->next_chain = 0; // clear chain after chain is done

}




struct ChainOperations chainOperationGet(struct ChainBase *Base)
{

    return Base->operations;

}



void initChainBase(struct ChainBase *Base, struct UnixSocket *Socket)
{

    Base->operations.addHandle = addHandle;
    Base->operations.handle = handle;
    Base->soket = Socket;

    Base->next_chain = 0;

}


void deinitChainBase(struct ChainBase *Base)
{

}


