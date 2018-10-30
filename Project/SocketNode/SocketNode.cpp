//
// Created by patteliu on 2018/10/25.
//

#include "SocketNode.h"

#define TCP_MSS 1460

CSocketNode::CSocketNode()
{
    m_Socket = nullptr;
    m_Loop = nullptr;
    m_Closed = false;
}

CSocketNode::~CSocketNode()
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

void CSocketNode::Init(IEventLoop *Loop, IIOSocket *Socket)
{
    m_Loop = Loop;
    m_Socket = Socket;

    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->AddClient(m_Socket->GetSocket(), this);
}

void CSocketNode::NodeClose()
{
    CNode::NodeClose();

    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->Remove(m_Socket->GetSocket());
}

bool CSocketNode::DataOut(const void *Buffer, size_t Length)
{
    if (m_Loop == nullptr || m_Socket == nullptr)
        return false;

    if (Length == 0)
        return true;

    ssize_t WriteLen = m_Socket->Send(Buffer, Length, MSG_NOSIGNAL);

    if (WriteLen <= 0)
        return false;

    if (WriteLen < Length)
    {
        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer + WriteLen, (u_char *)Buffer + Length);

        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_WRITE | EV_CLOSED | EV_PERSIST);
    }

    return true;
}

void CSocketNode::OnRead(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    u_char Buffer[TCP_MSS] = {};

    ssize_t ReadLen = m_Socket->Recv(Buffer, TCP_MSS, 0);

    if (ReadLen <= 0)
    {
        NodeClose();
        return;
    }

    bool NodeListStatus = DataIn(Buffer, ReadLen);

    if (!NodeListStatus)
        NodeClose();
}

void CSocketNode::OnWrite(int fd, short Event)
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

void CSocketNode::OnClose(int fd, short Event)
{
    if (m_Closed)
        return;

    m_Closed = true;

    NodeClose();
}