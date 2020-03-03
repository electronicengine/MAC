/**
 * subject.c  2014-05-14
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
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "subject.h"
#include "macsocket.h"


static int subjectOpsRegisterObserver(struct Subject *Subject, struct Observer *Observer)
{
    Subject->observers[Subject->observer_queue++] = Observer;

	return 0;
}



static int subjectOpsRemoveObserver(struct Subject *Subject, struct Observer *Observer)
{

    for (int i=0; i< Subject->observer_queue; ++i)
    {
        if (Observer == Subject->observers[i])
        {
            Subject->observers[i] = 0;
			break;
		}
	}

    for (int i = 0; i<Subject->observer_queue-1; ++i)
    {
        Subject->observers[i] = Subject->observers[i+1];
        Subject->observers[i+1] = 0;
	}

    return 0;
}



static int subjectOpsNotifyObservers(struct Subject *Subject, struct UnixSocket *Socket, ServiceMessage *Message)
{

    for (int i = 0; i < Subject->observer_queue; ++i)
    {
        if (Subject->observers[i])
        {
            Subject->observers[i]->operation.update(Subject->observers[i], Socket, Message);
        }
	}

	return 0;
}



static struct SubjectOperations SubjectOperation =
{
    .registerObserver = subjectOpsRegisterObserver,
    .removeObserver = subjectOpsRemoveObserver,
    .notifyObservers = subjectOpsNotifyObservers,
};



/** constructor(). */
void initSubject(struct Subject *Subject)
{
    memset(Subject, sizeof(*Subject), 0);
    Subject->operations = SubjectOperation;
    Subject->observer_queue = 0;
}
