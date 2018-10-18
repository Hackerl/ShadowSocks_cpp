//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETPIPE_H
#define SHADOWSOCKSR_CPP_SOCKETPIPE_H

#include "Pipe/IPipe.h"

class CPipe : public IPipe
{
public:
    CPipe();

public:
    bool PipeIn(const char * Buffer, size_t Length) override;
    virtual bool PipeOut(const char * Buffer, size_t Length) = 0;
    void PipeConnect(IPipe * PipePort) override;
    void PipeClose() override;

public:
    virtual void OnPipeClose() = 0;

public:
    IPipe * GetPipePort();

protected:
    bool m_PipeClosed;
    IPipe * m_PipePort;
};
#endif //SHADOWSOCKSR_CPP_SOCKETPIPE_H
