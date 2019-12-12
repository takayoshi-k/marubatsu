/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include "CSynchronized.h"

// constructor and destructor
CSynchronized::CSynchronized(
    CSynchronizer *sync)
: fSync(sync)
{
    fSync->lock();
}

CSynchronized::~CSynchronized()
{
    fSync->unlock();
}

// public member methods.
void CSynchronized::wait()
{
    fSync->wait();
}

void CSynchronized::notifyAll()
{
    fSync->notifyAll();
}
