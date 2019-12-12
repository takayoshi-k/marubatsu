/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include <iostream>
#include <unistd.h>
#include <assert.h>
#include "CSynchronized.h"
#include "CCommandDispatcher.h"

// constructor and destructor
CCommandDispatcher::CCommandDispatcher()
: fError(true)
, fHasTerminated(false)
, fQueueSync(0)
, fThreadSync(0)
, fQueue(0)
, fThread(0)
, isShared(false)
, fState(STATE_IDLE)
{
    fQueueSync = CSynchronizer::createInstance();
    if (fQueueSync == 0) {
        ::std::cout << 
            "Failed CSynchronizer::createInstance()" << ::std::endl;
        assert(false);
        return;
    }

    fThreadSync = CSynchronizer::createInstance();
    if (fThreadSync == 0) {
        ::std::cout << 
            "Failed CSynchronizer::createInstance()" << ::std::endl;
        assert(false);
        return;
    }

    fQueue = new CQueue(fQueueSync);

    fThread = createStartedThread(this);
    if (fThread == 0) {
        return;
    }

    fError = false;
}

// constructor and destructor
CCommandDispatcher::CCommandDispatcher(CCommandDispatcher *shareable)
: fError(true)
, fHasTerminated(false)
, fQueueSync(0)
, fQueue(0)
, fThread(0)
, isShared(true)
, fState(STATE_IDLE)
{
    fQueueSync = shareable->fQueueSync;

    fThreadSync = CSynchronizer::createInstance();
    if (fThreadSync == 0) {
        ::std::cout << 
            "Failed CSynchronizer::createInstance()" << ::std::endl;
        assert(false);
        return;
    }

    fQueue = shareable->fQueue;

    fThread = createStartedThread(this);
    if (fThread == 0) {
        return;
    }

    fError = false;
}


CCommandDispatcher::~CCommandDispatcher()
{
    if (fThread != 0) {
        fThreadSync->lock();
        while (fState != STATE_IDLE){
          fThreadSync->wait();
        }
        fThread->cancel();    // Terminate the thread.
        fThreadSync->unlock();
        fThread->join();
    }
    delete fThread;
    delete fThreadSync;

    if (!isShared) {
        delete fQueue;
        delete fQueueSync;
    }
}

void CCommandDispatcher::waitIdle()
{
    fThreadSync->lock();
    while (fState != STATE_IDLE){
        fThreadSync->wait();
    }
    fThreadSync->unlock();
}

// public member methods.
CCommandDispatcher *CCommandDispatcher::createInstance()
{
    CCommandDispatcher *instance = new CCommandDispatcher();
    if (instance->hasErrorOccurred()) {
        delete instance;
        return 0;
    }
    return instance;
}

CCommandDispatcher *CCommandDispatcher::createShareInstance(CCommandDispatcher *shareable)
{
    CCommandDispatcher *instance = new CCommandDispatcher(shareable);
    if (instance->hasErrorOccurred()) {
        delete instance;
        return 0;
    }
    return instance;
}

void CCommandDispatcher::pushBack(
    ICommand *command)
{
    fQueue->pushBack(command);
}

void CCommandDispatcher::waitForAllCommandsExecuted()
{
    fQueue->waitForEmpty();
    waitIdle();
}

// IRunnable's method.
void CCommandDispatcher::run()
{
    while (! fHasTerminated) {
        ICommand *command = fQueue->getFront(this);
        command->execute();
        delete command;

        fThreadSync->lock();
        fState = STATE_IDLE;
        fThreadSync->unlock();
        fThreadSync->notifyAll();

        // fQueue->removeFront();
    }
}

void CCommandDispatcher::changeStateIdle()
{
    fThreadSync->lock();
    fState = STATE_EXECUTING;
    fThreadSync->unlock();
}


// private member method.
CThread *CCommandDispatcher::createStartedThread(
    IRunnable *runnable)
{
    CThread *thread = CThread::createInstance(runnable);
    if (thread == 0) {
        ::std::cout << 
            "Failed CThread::createInstance()" << ::std::endl;
        assert(false);
        return 0;
    }
    if (! thread->start()) {
        ::std::cout << "Failed CThread::start()" << ::std::endl;
        assert(false);
        delete thread;
        return 0;
    }
    return thread;
}

bool CCommandDispatcher::hasErrorOccurred()
{
    return fError;
}

/*****************************************/
/* CCommandDispatcher::CTerminateCommand */
/*****************************************/
// constructor and destructor
CCommandDispatcher::CTerminateCommand::CTerminateCommand(
    bool &hasTerminated)
: fHasTerminated(hasTerminated)
{
    // Nothing to do.
}

// public member methods.
// ICommand's method.
void CCommandDispatcher::CTerminateCommand::execute()
{
    fHasTerminated = true;
}

/******************************/
/* CCommandDispatcher::CQueue */
/******************************/
// constructor and destructor
CCommandDispatcher::CQueue::CQueue(
    CSynchronizer *sync)
: fSync(sync)
, fCommands()
{
    // Nothing to do.
}

CCommandDispatcher::CQueue::~CQueue()
{
    assert(fCommands.empty());
}

// public member methods.
ICommand *CCommandDispatcher::CQueue::getFront(CCommandDispatcher *disp) 
{
    ICommand *ret;

    {
        CSynchronized sync(fSync);
        while (fCommands.empty()) {
            sync.wait();
        }
        ret = fCommands.front();
        disp->changeStateIdle();
        fCommands.pop_front();
        sync.notifyAll();
    }
    return ret;
}

void CCommandDispatcher::CQueue::removeFront()
{
    CSynchronized sync(fSync);
    if  (fCommands.empty()) {
        ::std::cout << "Failed removeFront() [Queue is empty]" << ::std::endl;
        assert(false);
        return;
    }
    ICommand *command = fCommands.front();
    fCommands.pop_front();
    delete command;
    sync.notifyAll();
}

void CCommandDispatcher::CQueue::pushBack(
    ICommand *command)
{
    CSynchronized sync(fSync);
    fCommands.push_back(command);
    sync.notifyAll();
}

void CCommandDispatcher::CQueue::waitForEmpty()
{
    CSynchronized sync(fSync);
    while (! fCommands.empty()) {
        sync.wait();
    }
}

