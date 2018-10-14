//
// Created by hackerl on 10/14/18.
//

#include "SocketPipe.h"

CSocketPipe::CSocketPipe() {
    m_PipePort = nullptr;
    m_PipeClosed = true;
}

CSocketPipe::CSocketPipe(ISocketPipe * PipePort) {
    m_PipePort = PipePort;
    m_PipeClosed = false;
}

void CSocketPipe::SetPipePort(ISocketPipe *PipePort) {
    m_PipePort = PipePort;
    m_PipeClosed = false;
}

bool CSocketPipe::PipeIn(const char *Buffer, size_t Length) {
    if (m_PipePort == nullptr || m_PipeClosed)
        return false;

    m_PipePort->PipeOut(Buffer, Length);

    return true;
}

void CSocketPipe::PipeClose() {
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
