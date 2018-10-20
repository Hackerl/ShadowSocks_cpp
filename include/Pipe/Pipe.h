//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETPIPE_H
#define SHADOWSOCKSR_CPP_SOCKETPIPE_H

#include "Pipe/IPipe.h"
#include "Common/IInstanceManager.h"

class CPipe : public IPipe
{
public:
    CPipe()
    {
        m_PipePort = nullptr;
        m_PipeClosed = true;
    }

public:
    bool PipeIn(const char * Buffer, size_t Length) override
    {
        if (m_PipePort == nullptr || m_PipeClosed)
            return false;

        m_PipePort->PipeOut(Buffer, Length);

        return true;
    }

    virtual bool PipeOut(const char * Buffer, size_t Length) = 0;

    void PipeConnect(IPipe * PipePort) override
    {
        m_PipePort = PipePort;
        m_PipeClosed = false;
        AddRef(PipePort);
    }

    void PipeClose() override
    {
        if (m_PipeClosed)
            return;

        m_PipeClosed = true;

        if (m_PipePort != nullptr)
        {
            m_PipePort->PipeClose();
            Release(m_PipePort);
            m_PipePort = nullptr;
        }

        OnPipeClose();
    }

public:
    virtual void OnPipeClose() = 0;

public:
    IPipe * GetPipePort()
    {
        return m_PipePort;
    }

protected:
    bool m_PipeClosed;
    IPipe * m_PipePort;
};
#endif //SHADOWSOCKSR_CPP_SOCKETPIPE_H