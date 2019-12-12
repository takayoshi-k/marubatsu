/** 
 * Copyright (c) 2019 N.Tsuyuzaki All rights reserved.
 */
#ifndef _I_COMMAND_H
#define _I_COMMAND_H

/**
 * CCommandDispatcherにキューイングされるインスタンスが継承するインターフェースです。
 * ICommandを継承したクラスのexecute()にCCommandDispatcherのワーカースレッドで処理したい実装を定義します。
 */
class ICommand
{
public:
    virtual ~ICommand() { }
    
    /**
     * CCommandDispatcherのワーカースレッドで呼び出されるメソッド。
     */
    virtual void execute() = 0;

};

#endif /* _I_COMMAND_H */
