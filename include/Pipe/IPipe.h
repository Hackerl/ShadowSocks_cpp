//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_IPIPE_H
#define SHADOWSOCKSR_CPP_IPIPE_H

#include <iostream>
#include "Common/Interface.h"

class IPipeNode : public Interface
{
public:
    virtual void PipeInit(void * args) = 0;
    virtual bool PipeIn(const void *Buffer, size_t Length) = 0;
    virtual bool PipeOut(const void *Buffer, size_t Length) = 0;
    virtual void PipeClose() = 0;
};

class IPipe : public IPipeNode
{
public:
    void PipeInit(void * args) override = 0;
    bool PipeIn(const void *Buffer, size_t Length) override = 0;
    bool PipeOut(const void *Buffer, size_t Length) override = 0;
    void PipeClose() override = 0;
    virtual void OnPipeClose() = 0;
    virtual bool OnDataIn(const void *Buffer, size_t Length) = 0;
    virtual bool OnPipeIn(const void *Buffer, size_t Length) = 0;
    virtual void PipeConnect(IPipe * Pipe) = 0;
};

template <class T>
class IDelayInstance : public Interface
{
public:
    virtual bool InitDelay(T & arg) = 0;
};

template <class T>
inline void PairPipeConnect(T * X, T * Y)
{
    auto PipeX = dynamic_cast<IPipe *>(X);
    auto PipeY = dynamic_cast<IPipe *>(Y);

    if (PipeX != nullptr && PipeY != nullptr)
    {
        PipeX->PipeConnect(PipeY);
        PipeX->PipeConnect(PipeX);
    }
};

#endif //SHADOWSOCKSR_CPP_IPIPE_H
