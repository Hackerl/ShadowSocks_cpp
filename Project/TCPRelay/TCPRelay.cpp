//
// Created by hackerl on 10/14/18.
//

#include "TCPRelay.h"

#define TCP_MSS 1460

CTCPRelay::CTCPRelay()
{
    m_Socket = nullptr;
    m_Loop = nullptr;
}

void CTCPRelay::PipeInit(void *args)
{
    if (m_Socket != nullptr)
        return;

    m_Socket = static_cast<IIOSocket *>(args);

    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->AddClient(m_Socket->GetSocket(), this);
}

void CTCPRelay::Init(IIOSocket *Socket, IEventLoop *Loop)
{
    m_Socket = Socket;
    m_Loop = Loop;
}

void CTCPRelay::Init(IEventLoop *Loop)
{
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

bool CTCPRelay::PipeOut(const void *Buffer, size_t Length)
{
    if (m_Loop == nullptr || m_Socket == nullptr)
        return false;

    ssize_t WriteLen = m_Socket->Send(Buffer, Length);

    if (WriteLen < 0)
        return false;

    if (WriteLen < Length)
    {
        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer + WriteLen, (u_char *)Buffer + Length);

        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_WRITE | EV_CLOSED | EV_PERSIST);
    }

    return true;
}

void CTCPRelay::OnPipeClose()
{
    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->Remove(m_Socket->GetSocket());
}

void CTCPRelay::OnRead(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    u_char Buffer[TCP_MSS] = {};

    ssize_t ReadLen = m_Socket->Recv(Buffer, TCP_MSS, 0);

    if (ReadLen > 0)
        OnDataIn(Buffer, ReadLen);
}

void CTCPRelay::OnWrite(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    if (m_WriteBuffer.empty())
        return;

    size_t BufferSize = m_WriteBuffer.size();

    ssize_t WriteLen = m_Socket->Send(m_WriteBuffer.data(), BufferSize < TCP_MSS ? BufferSize : TCP_MSS);

    if (WriteLen > 0)
        m_WriteBuffer.erase(m_WriteBuffer.begin(), m_WriteBuffer.begin() + WriteLen);

    if (m_WriteBuffer.empty())
        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_CLOSED | EV_PERSIST);
}

void CTCPRelay::OnClose(int fd, short Event)
{
    PipeClose();
}
