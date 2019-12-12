/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _I_RUNNABLE_H
#define _I_RUNNABLE_H

/**
 * CThread::createInstance()に渡すインスタンスが継承するインターフェースです。
 * IRunnableを継承したクラスのrun()メソッドに別スレッドで処理したい実装を定義します。
 */
class IRunnable
{
public:
    virtual ~IRunnable() { }

    /**
     * CThreadクラスのstartメソッドで起動したスレッドで実行されるメソッドです。
     */
    virtual void run() = 0;
};

#endif /* _I_RUNNABLE_H */
