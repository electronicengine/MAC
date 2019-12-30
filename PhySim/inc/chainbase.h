#ifndef __chainbase_H
#define __chainbase_H
#include <inttypes.h>


#include <stdio.h>
#include <stdlib.h>
//#include "datasap.h"
//#include "managementsap.h"

struct ChainBase;


struct ChainOperations
{

        void (*addHandle)(struct ChainBase *Base, struct ChainBase *Next); /* not override */
        int (*handle)(struct ChainBase *, uint8_t *, uint8_t);
        void (*close)(struct ChainBase *); /* virtual desturct routine, the base class no-implement */

};



struct ChainBase
{

    struct ChainOperations operations;
    struct ChainBase *next_chain; // 1. "next" pointer in the base class
    struct UnixSocket *soket;

};



void initChainBase(struct ChainBase *Base, struct UnixSocket *Socket); /* construct routine */
void deinitChainBase(struct ChainBase *Base);


struct ChainOperations chainOperationGet(struct ChainBase *Base); /* base::ops */



#endif
