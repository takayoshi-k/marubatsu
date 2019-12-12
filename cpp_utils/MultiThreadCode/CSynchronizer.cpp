/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#include <iostream>
#include <assert.h>
#include "CSynchronizer.h"

// constructor
CSynchronizer::CSynchronizer()
: fError(true)
, fMutex()
, fMutexAttr()
, fCond()
{
    int result = pthread_mutexattr_init(&fMutexAttr);
    if (result != 0) {
        ::std::cout << "pthread_mutexattr_init() returns " 
            << result << ::std::endl;
        assert(false);
        return;
    }
    
    result = pthread_mutexattr_settype(
        &fMutexAttr, 
        PTHREAD_MUTEX_RECURSIVE);
    if (result != 0) {
        ::std::cout << "pthread_mutexattr_settype() returns " 
            << result << ::std::endl;
        assert(false);
        return;
    }
    
    result = pthread_mutex_init(&fMutex, &fMutexAttr);
    if (result != 0) {
        ::std::cout << "pthread_mutex_init() returns " 
            << result << ::std::endl;
        assert(false);
        return;
    }

    result = pthread_cond_init(&fCond, NULL);
    if (result != 0) {
        ::std::cout << "pthread_cond_init() returns " 
            << result << ::std::endl;
        assert(false);
        return;
    }

    fError = false;
}

CSynchronizer::~CSynchronizer()
{
    int result = pthread_cond_destroy(&fCond);
    assert(result == 0);
    result = pthread_mutex_destroy(&fMutex);
    assert(result == 0);
    result = pthread_mutexattr_destroy(&fMutexAttr);
    assert(result == 0);
}

// public member methos.
CSynchronizer *CSynchronizer::createInstance()
{
    CSynchronizer *instance = new CSynchronizer();
    if (instance->hasErrorOccurred()) {
        delete instance;
        return 0;
    }
    return instance;
}

void CSynchronizer::lock()
{
    int result = pthread_mutex_lock(&fMutex);
    if (result != 0) {
        ::std::cout << "pthread_mutex_lock() returns " 
            << result << ::std::endl;
        assert(false);
    }
}
    
void CSynchronizer::unlock()
{
    int result = pthread_mutex_unlock(&fMutex);
    if (result != 0) {
        ::std::cout << "pthread_mutex_lock() returns " 
            << result << ::std::endl;
        assert(false);
    }
}
    
void CSynchronizer::wait()
{
    int result = pthread_cond_wait(&fCond, &fMutex);
    if (result != 0) {
        ::std::cout << "pthread_cond_wait() returns " 
            << result << ::std::endl;
        assert(false);
    }
}

void CSynchronizer::notifyAll()
{
    int result = pthread_cond_broadcast(&fCond);
    if (result != 0) {
        ::std::cout << "pthread_cond_broadcast() returns " 
            << result << ::std::endl;
        assert(false);
    }
}

// private member methos.
bool CSynchronizer::hasErrorOccurred()
{
    return fError;
}

