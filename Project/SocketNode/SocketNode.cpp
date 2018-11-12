//
// Created by patteliu on 2018/10/25.
//

#include "SocketNode.h"
#include <netinet/tcp.h>

#define USER_TCP_MSS 1460

CSocketNode::CSocketNode()
{
    m_Socket = nullptr;
    m_Loop = nullptr;
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

void CSocketNode::SocketNodeInit(IEventLoop *Loop, IIOSocket *Socket)
{
    m_Loop = Loop;
    m_Socket = Socket;

    if (m_Loop == nullptr || m_Socket == nullptr)
        return;

    int OptVal = 1;

    m_Socket->SetSockOpt(IPPROTO_TCP, TCP_NODELAY, &OptVal, sizeof(OptVal));

    m_Loop->AddClient(m_Socket->GetSocket(), this);
}

bool CSocketNode::DataOut(const void *Buffer, size_t Length)
{
    if (m_Loop == nullptr || m_Socket == nullptr)
        return false;

    if (Length == 0)
        return true;

    if (!m_WriteBuffer.empty())
    {
        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer, (u_char *)Buffer + Length);
        return true;
    }

    ssize_t WriteLen = m_Socket->Send(Buffer, Length, MSG_NOSIGNAL | MSG_DONTWAIT);

    if (WriteLen <= 0 && errno != EAGAIN)
        return false;

    if (WriteLen < Length)
    {
        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer + WriteLen, (u_char *)Buffer + Length);

        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_WRITE | EV_CLOSED | EV_PERSIST);

        BroadcastEvent(PIPE_STREAM_BLOCK, nullptr, this);
    }

    return true;
}

void CSocketNode::OnRead(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    u_char Buffer[USER_TCP_MSS] = {};

    ssize_t ReadLen = m_Socket->Recv(Buffer, USER_TCP_MSS, 0);

    if (ReadLen <= 0)
    {
        OnClose(m_Socket->GetSocket(), EV_CLOSED);
        return;
    }

    bool NodeListStatus = DataIn(Buffer, ReadLen);

    if (!NodeListStatus)
        OnClose(m_Socket->GetSocket(), EV_CLOSED);
}

void CSocketNode::OnWrite(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    if (m_WriteBuffer.empty())
        return;

    size_t BufferSize = m_WriteBuffer.size();

    ssize_t WriteLen = m_Socket->Send(m_WriteBuffer.data(), BufferSize, MSG_NOSIGNAL | MSG_DONTWAIT);

    if (WriteLen > 0)
    {
        m_WriteBuffer.erase(m_WriteBuffer.begin(), m_WriteBuffer.begin() + WriteLen);
        m_WriteBuffer.shrink_to_fit();
    }

    if (m_WriteBuffer.empty())
    {
        BroadcastEvent(PIPE_STREAM_FLOW, nullptr, this);
        m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_CLOSED | EV_PERSIST);
    }
}

bool CSocketNode::NodeInit(INodeManager *NodeManager)
{
    CNode::NodeInit(NodeManager);

    return RegisterEvent(PIPE_STREAM_BLOCK, this) && RegisterEvent(PIPE_STREAM_FLOW, this);
}

void CSocketNode::OnNodeEvent(unsigned int EventID, void * Context)
{
    switch (EventID)
    {
        case PIPE_STREAM_BLOCK:
            m_Loop->SetEvent(m_Socket->GetSocket(), EV_CLOSED | EV_PERSIST);
            break;

        case PIPE_STREAM_FLOW:
            m_Loop->SetEvent(m_Socket->GetSocket(), EV_READ | EV_CLOSED | EV_PERSIST);
            break;
    }
}

void CSocketNode::NodeClose()
{
    CNode::NodeClose();

    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->Remove(m_Socket->GetSocket());
}

void CSocketNode::OnClose(int fd, short Event)
{
    BroadcastEvent(NODE_CLOSE_EVENT, nullptr, this);
}