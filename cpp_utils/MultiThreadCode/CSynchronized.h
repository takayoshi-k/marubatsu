/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _C_SYNCHRONIZED_H
#define _C_SYNCHRONIZED_H

#include "CSynchronizer.h"

/**
 * CSynchronizerのラッパークラスです。
 * CSynchronizedのコンストラクタにはCSynchronizerインスタンスのポインタを渡します。
 * コンストラクタでCSynchronizerのlock()を行い、デストラクタでunlock()を行います。
 */
class CSynchronized
{
public:
    CSynchronized(
        CSynchronizer *sync);

    virtual ~CSynchronized();

    /**
     * CSynchronizerのwait()を呼び出します。
     */
    void wait();

    /**
     * CSynchronizerのnotifyAll()を呼び出します。
     */
    void notifyAll();

private:
    CSynchronizer *fSync;

};

#endif /* _C_SYNCHRONIZED_H */
