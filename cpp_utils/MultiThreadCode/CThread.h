/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_THREAD_H
#define _C_THREAD_H

#include <pthread.h>
#include "IRunnable.h"
#include "CSynchronizer.h"

/**
 * CThreadクラスはインスタンスごとにスレッドを起動します。
 * 起動したスレッドでコンストラクタの引数で渡されたIRunnableのrun()メソッドをcallします。
 */
class CThread
{
private:
    CThread(
        IRunnable *runnable);

public:
    virtual ~CThread();

    /**
     * CThreadのインスタンスを生成します。
     * 引数で渡したrunnableのrun()メソッドを別スレッドでcallします。
     * インスタンスの生成に失敗すると 0 が返ります。
     */
    static CThread *createInstance(
        IRunnable *runnable);

    /**
     * 新しいスレッドを起動します。
     * 1インスタンスで複数回呼び出すとassertします。
     */
    bool start();

    /**
     * 起動したスレッドの完了を待ち合わせます。
     * スレッドを起動したのち、呼び出しが可能です。
     * 1インスタンスで複数回呼び出すとassertします。
     */
    void join();

    void cancel();

private:
    static void *doRun(
        void *args);

    void callRun();

    bool initialize();

    bool hasErrorOccurred();

    bool fError;
    bool fHasInitialized;
    pthread_t fPthread; 
    IRunnable *fRunnable;
    CSynchronizer *fSync;
};

#endif /* _C_THREAD_H */
