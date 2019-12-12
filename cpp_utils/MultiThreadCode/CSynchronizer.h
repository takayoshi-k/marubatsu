/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_SYNCHRONIZER_H
#define _C_SYNCHRONIZER_H

#include <pthread.h>

/**
 * スレッドの排他を行うためのクラスです。
 */
class CSynchronizer
{
private:
    CSynchronizer();

public:
    virtual  ~CSynchronizer();
    
    /**
     * CSynchronizerインスタンスを生成します。
     * インスタンスの生成に失敗すると 0 が返ります。
     */
    static CSynchronizer *createInstance();

    /**
     * 相互排他lockを行います。
     */
    void lock();
    
    /**
     * 相互排他lockをunlockします。
     */
    void unlock();
    
    /**
     * 待ち合わせ状態に遷移します。
     */
    void wait();

    /**
     * 待ち合わせ状態を解除するシグナルをbroadcastします。
     */
    void notifyAll();

private:
    bool hasErrorOccurred();

    bool fError;
    pthread_mutex_t fMutex;
    pthread_mutexattr_t fMutexAttr;
    pthread_cond_t fCond;
};

#endif /* _C_SYNCHRONIZER_H */
