/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_COMMAND_DISPATCHER_H
#define _C_COMMAND_DISPATCHER_H

#include <list>
#include "ICommand.h"
#include "IRunnable.h"
#include "CThread.h"
#include "CSynchronizer.h"

/**
 * ICommandをキューイングし、ワーカースレッドでICommandのexecute()を逐次実行するクラスです。
 * ワーカースレッド数は1です。
 * キューイングしたICommandはCCommandDispatcherがdeleteします。クライアント側でdeleteしないでください。
 */
class CCommandDispatcher:
    public IRunnable
{
private:
    CCommandDispatcher();
    CCommandDispatcher(CCommandDispatcher *shareable);

    enum _state {
      STATE_IDLE,
      STATE_EXECUTING
    };

public:
    virtual ~CCommandDispatcher();

    /**
     * CCommandDispatcherのインスタンスを生成します。
     * インスタンスの生成に失敗すると 0 が返ります。
     */
    static CCommandDispatcher *createInstance();
    static CCommandDispatcher *createShareInstance(CCommandDispatcher *shareable);
    
    /**
     * ICommandをキューイングします。
     * キューイングされたICommandは逐次ワーカースレッドでexecute()呼び出しがされます。
     * キューイングしたICommandは、execute()呼び出しが終わるとCCommandDispatcherによりdeleteされます。
     */
    void pushBack(
        ICommand *command);
    
    /**
     * キューイングされているICommandのexecute()呼び出しが全て完了するのを待ち合わせます。
     */
    void waitForAllCommandsExecuted();

    // IRunnable's method.
    virtual void run();

    void changeStateIdle();

private:
    class CTerminateCommand:
        public ICommand
    {
    public:
        CTerminateCommand(
            bool &hasTerminated);

        // ICommand's method.
        virtual void execute();

    private:
         bool &fHasTerminated;

    };

    class CQueue
    {
    public:
        CQueue(
            CSynchronizer *sync);

        virtual ~CQueue();

        ICommand *getFront(CCommandDispatcher *disp);

        void removeFront();

        void pushBack(
            ICommand *command);

        void waitForEmpty();

    private:
        ::std::list<ICommand *> fCommands;
        CSynchronizer *fSync;
    };
    
    static CThread *createStartedThread(
        IRunnable *runnable);

    bool hasErrorOccurred();

    void waitIdle();

    bool fError;
    bool fHasTerminated;
    CSynchronizer *fQueueSync;
    CSynchronizer *fThreadSync;
    CQueue *fQueue;
    CThread *fThread;
    bool isShared;
    volatile _state fState;
};

#endif /* _C_COMMAND_DISPATCHER_H */
