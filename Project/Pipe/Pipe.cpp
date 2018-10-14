//
// Created by hackerl on 10/14/18.
//

#include "Pipe/Pipe.h"

CPipe::CPipe() {
    m_PipePort = nullptr;
    m_PipeClosed = true;
}

void CPipe::PipeConnect(IPipe * PipePort) {
    m_PipePort = PipePort;
    m_PipeClosed = false;
}

bool CPipe::PipeIn(const char *Buffer, size_t Length) {
    if (m_PipePort == nullptr || m_PipeClosed)
        return false;

    m_PipePort->PipeOut(Buffer, Length);

    return true;
}

void CPipe::PipeClose() {
    if (m_PipeClosed)
        return;

    m_PipeClosed = true;

    if (m_PipePort != nullptr)
    {
        m_PipePort->PipeClose();
        m_PipePort = nullptr;
    }

    OnPipeClose();
}

IPipe *CPipe::GetPipePort() {
    return m_PipePort;
}
