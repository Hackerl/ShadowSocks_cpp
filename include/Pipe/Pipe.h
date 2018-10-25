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
    void PipeInit(void * args) override = 0;

public:
    bool PipeIn(const void *Buffer, size_t Length) override
    {
        if (m_PipePort == nullptr || m_PipeClosed)
            return false;

        return m_PipePort->OnPipeIn(Buffer, Length);
    }

    bool PipeOut(const void *Buffer, size_t Length) override = 0;

public:
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

        OnPipeClose();

        if (m_PipePort != nullptr)
        {
            IPipe * TmpPtr = m_PipePort;
            m_PipePort->PipeClose();
            Release(TmpPtr);
            m_PipePort = nullptr;
        }
    }

public:
    void OnPipeClose() override = 0;

    bool OnDataIn(const void *Buffer, size_t Length) override
    {
        return PipeIn(Buffer, Length);
    }

    bool OnPipeIn(const void *Buffer, size_t Length) override
    {
        return PipeOut(Buffer, Length);
    }

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