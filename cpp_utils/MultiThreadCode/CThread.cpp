/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include <assert.h>
#include <iostream>
#include "CSynchronized.h"
#include "CThread.h"

// constructor and destructor
CThread::CThread(
    IRunnable *runnable)
: fError(true)
, fHasInitialized(false)
, fPthread()
, fRunnable(runnable)
, fSync(0)
{
    fSync = CSynchronizer::createInstance();
    if (fSync == 0) {
        assert(false);
        return;
    }

    fError = false;
}

CThread::~CThread()
{
    delete fSync;
}

// public member methods.
CThread *CThread::createInstance(
    IRunnable *runnable)
{
    CThread *instance = new CThread(runnable);
    if (instance->hasErrorOccurred()) {
        delete instance;
        return 0;
    }
    return instance;
}

bool CThread::start()
{
    return initialize();
}

void CThread::cancel()
{
    int result = pthread_cancel(fPthread);
    if (result != 0) {
        ::std::cout << 
            "pthread_cancel() returns " << result << ::std::endl;
        assert(false);
    }
}

void CThread::join()
{
    int result = pthread_join(fPthread, NULL);
    if (result != 0) {
        ::std::cout << 
            "pthread_join() returns " << result << ::std::endl;
        assert(false);
    }
}

// private member methods.
void *CThread::doRun(
    void *args)
{
    CThread *thread = (CThread *)(args);
    thread->callRun();
    return NULL;
}

void CThread::callRun()
{
    fRunnable->run();
}

bool CThread::initialize()
{
    CSynchronized sync(fSync);
    if (fHasInitialized) {
        ::std::cout << 
            "already initialized" << ::std::endl;
        assert(false);
        return false;
    }
    int result = pthread_create(
            &fPthread, 
            NULL, 
            doRun, 
            this);
    if (result != 0) {
        ::std::cout << 
            "pthread_create() returns " << result << ::std::endl;
        assert(false);
        return false;
    }
    fHasInitialized = true;
    return true;
}

bool CThread::hasErrorOccurred()
{
    return fError;
}
