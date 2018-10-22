//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_IPIPE_H
#define SHADOWSOCKSR_CPP_IPIPE_H

#include <iostream>
#include "Common/Interface.h"

class IPipe : public Interface
{
public:
    virtual bool PipeIn(const void *Buffer, size_t Length) = 0;
    virtual bool PipeOut(const void *Buffer, size_t Length) = 0;
    virtual void PipeConnect(IPipe * Pipe) = 0;
    virtual void PipeClose() = 0;
};

template <class T>
class IDelayInstance : public Interface
{
public:
    virtual bool InitDelay(T & arg) = 0;
};

#endif //SHADOWSOCKSR_CPP_IPIPE_H
