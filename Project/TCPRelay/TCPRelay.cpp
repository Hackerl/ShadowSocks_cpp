//
// Created by hackerl on 10/14/18.
//

#include "TCPRelay.h"

CTCPRelay::CTCPRelay()
{
    m_Socket = nullptr;
    m_Loop = nullptr;
}

void CTCPRelay::Init(IIOSocket *Socket, IEventLoop *Loop)
{
    m_Socket = Socket;
    m_Loop = Loop;
}

CTCPRelay::~CTCPRelay()
{
    if (m_Socket != nullptr)
    {
        m_Socket->Close();
        delete m_Socket;
        m_Socket = nullptr;
    }

    m_Loop = nullptr;

    m_WriteBuffer.clear();
}

bool CTCPRelay::PipeOut(const char *Buffer, size_t Length)
{
    if (m_Loop == nullptr)
        return false;

    m_WriteBuffer.insert(m_WriteBuffer.end(), Buffer, Buffer + Length);

    m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_WRITE | EV_CLOSED | EV_PERSIST);

    return true;
}

void CTCPRelay::OnPipeClose()
{
    if (m_Loop != nullptr)
        m_Loop->Remove(m_Socket->GetSocket());
}

void CTCPRelay::OnRead(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    char Buffer[1024] = {};

    ssize_t len = m_Socket->Recv(Buffer, 1024, 0);

    if (len > 0)
        PipeIn(Buffer, len);
}

void CTCPRelay::OnWrite(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    if (m_WriteBuffer.empty())
        return;

    ssize_t len = m_Socket->Send(&m_WriteBuffer[0], m_WriteBuffer.size());

    if (len > 0)
    {
        m_WriteBuffer.erase(m_WriteBuffer.begin(), m_WriteBuffer.begin() + len);
    }

    if (m_WriteBuffer.empty())
    {
        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_CLOSED | EV_PERSIST);
    }
}

void CTCPRelay::OnClose(int fd, short Event)
{
    PipeClose();
}
