/**
 * subject.h  2014-05-14
 * anonymouse(anonymouse@email)
 *
 * Copyright (C) 2000-2014 All Right Reserved
 * 
 * THIS CODE AND INFORMATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY
 * KIND, EITHER EXPRESSED OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND/OR FITNESS FOR A
 * PARTICULAR PURPOSE.
 *
 * Auto generate for Design Patterns in C *
 * 
 * The Observer Pattern (sometimes called Listener Pattern) is a design
    pattern which establishes a one-to-many relationship between objects, such
    that changes to one object (the subject) may be communicated to many other
    objects (the observers/listeners). The power of this pattern is that it
    loosely couples objects together in a manner that does not require the
    subject or its listeners understanding anything about the other, beyond the
    fact that they implement a shared interface for communicating.
 */
#ifndef __SUBJECT_H__
#define __SUBJECT_H__

#include "observer.h"
#include "messagetypes.h"

#include <inttypes.h>



struct Subject;
struct MacSocket;


struct SubjectOperations
{

        int (*registerObserver)(struct Subject *, struct Observer *Observer);
        int (*removeObserver)(struct Subject *, struct Observer *Observer);
        int (*notifyObservers)(struct Subject *, struct MacSocket *Socket, ServiceMessage *Message, uint8_t *TransitData);

};



struct Subject
{


    struct SubjectOperations operations;
    struct Observer *observers[32];
    int observer_queue;

};



/** constructor(). */
void initSubject(struct Subject *Subject);







#endif /* __SUBJECT_H__ */
