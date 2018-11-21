//
// Created by patteliu on 2018/10/25.
//

#include "SocketNode.h"
#include <event.h>
#include <netinet/tcp.h>
#include <glog/logging.h>
#include <Node/NodeService.h>
#include <algorithm>

#define USER_TCP_MSS 1460UL
#define READ_MAX_SIZE (USER_TCP_MSS * 2)
#define SEND_MAX_SIZE (USER_TCP_MSS * 3)
#define MAX_SEND_BUFFER_SIZE (30 * USER_TCP_MSS)
#define MIN_SEND_BUFFER_SIZE (4 * USER_TCP_MSS)

CSocketNode::CSocketNode()
{
    m_Socket = nullptr;
    m_Loop = nullptr;
    m_Closed = false;

    m_Mode = EV_READ | EV_PERSIST;
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

    m_Loop->Add(m_Socket->GetSocket(), this);
}

bool CSocketNode::DataOut(const void *Buffer, size_t Length)
{
    if (m_Loop == nullptr || m_Socket == nullptr)
        return false;

    if (Length == 0)
        return true;

    if (!m_WriteBuffer.empty())
    {
        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Write Buffer Inset Size: " << Length;

        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer, (u_char *)Buffer + Length);

        if (m_WriteBuffer.size() > MIN_SEND_BUFFER_SIZE)
        {
            LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Write Buffer Size: " << m_WriteBuffer.size();

            BroadcastEvent(PIPE_STREAM_BLOCK, this);
        }

        return true;
    }

    ssize_t WriteLen = m_Socket->Send(Buffer, Length, MSG_NOSIGNAL | MSG_DONTWAIT);

    if (WriteLen <= 0 && errno != EAGAIN)
        return false;

    if (WriteLen != Length)
    {
        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Try Send Size: " << Length << " Real Send Size: " << WriteLen;

        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Write Buffer Inset Size: " << std::max(WriteLen, 0L);

        m_WriteBuffer.insert(m_WriteBuffer.end(), (u_char *)Buffer + std::max(WriteLen, 0L), (u_char *)Buffer + Length);
    }

    return true;
}

void CSocketNode::OnRead(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    if (Event == EV_TIMEOUT)
        return;

    u_char Buffer[READ_MAX_SIZE] = {};

    ssize_t ReadLen = m_Socket->Recv(Buffer, READ_MAX_SIZE, 0);

    if (ReadLen <= 0)
    {
        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Close Pipe. Receive Result: " << ReadLen;
        OnClose(m_Socket->GetSocket(), EV_READ);
        return;
    }

    bool NodeListStatus = DataIn(Buffer, ReadLen);

    if (!NodeListStatus)
    {
        LOG(WARNING) << "fd: " << m_Socket->GetSocket() << " Close Pipe. Pipe Flow Failed.";
        OnClose(m_Socket->GetSocket(), EV_READ);
    }
}

void CSocketNode::OnWrite(int fd, short Event)
{
    if (m_Socket == nullptr)
        return;

    if (Event == EV_TIMEOUT)
        BroadcastEvent(PIPE_STREAM_FLOW, this);

    if (m_WriteBuffer.size() > MAX_SEND_BUFFER_SIZE)
    {
        LOG(WARNING) << "fd: " << m_Socket->GetSocket() << " WriteBuffer Size Limit: " << m_WriteBuffer.size() << " Clear Buffer";

        m_WriteBuffer.clear();
        m_WriteBuffer.shrink_to_fit();
    }

    if (m_WriteBuffer.empty())
        return;

    size_t BufferSize = m_WriteBuffer.size();

    LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Write Buffer Size: " << BufferSize;

    size_t SendSize = std::min(BufferSize, SEND_MAX_SIZE);

    ssize_t WriteLen = m_Socket->Send(m_WriteBuffer.data(), SendSize, MSG_NOSIGNAL | MSG_DONTWAIT);

    LOG(INFO) << "fd: " << m_Socket->GetSocket() << " OnWrite Try Send Size: " << SendSize << " Real Send Size " << WriteLen;

    if (WriteLen > 0)
    {
        m_WriteBuffer.erase(m_WriteBuffer.begin(), m_WriteBuffer.begin() + WriteLen);
        m_WriteBuffer.shrink_to_fit();
    }

    if (m_WriteBuffer.empty())
        BroadcastEvent(PIPE_STREAM_FLOW, this);
}

bool CSocketNode::NodeInit(INodeManager *NodeManager)
{
    CNode::NodeInit(NodeManager);

    RegisterEvent(PIPE_STREAM_BLOCK, this);
    RegisterEvent(PIPE_STREAM_FLOW, this);

    return true;
}

void CSocketNode::OnNodeEvent(NodeEventRegister EventID, void *Context)
{
    if (m_Socket == nullptr)
        return;

    LOG(INFO) << "fd: " << m_Socket->GetSocket() << " OnNodeEvent: " << EventID;

    if (this == Context)
    {
        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Block Node Dispatch";

        switch (EventID)
        {
            case PIPE_STREAM_BLOCK:
                m_Loop->SetEvent(m_Socket->GetSocket(), m_Mode | short(EV_WRITE | EV_TIMEOUT), 20);
                break;

            case PIPE_STREAM_FLOW:
                m_Loop->SetEvent(m_Socket->GetSocket(), m_Mode & short(~(EV_WRITE | EV_TIMEOUT)));
                break;
        }
    }
    else
    {
        LOG(INFO) << "fd: " << m_Socket->GetSocket() << " Other Node Dispatch";

        switch (EventID)
        {
            case PIPE_STREAM_BLOCK:
                m_Loop->SetEvent(m_Socket->GetSocket(), m_Mode & short(~ EV_READ));
                break;

            case PIPE_STREAM_FLOW:
                m_Loop->SetEvent(m_Socket->GetSocket(), m_Mode | EV_READ);
                break;
        }
    }

}

void CSocketNode::NodeClose()
{
    CNode::NodeClose();

    if (m_WriteBuffer.size() > 0)
        m_Socket->Send(m_WriteBuffer.data(), m_WriteBuffer.size(), MSG_NOSIGNAL | MSG_DONTWAIT);

    if (m_Loop != nullptr && m_Socket != nullptr)
        m_Loop->Remove(m_Socket->GetSocket());
}

void CSocketNode::OnClose(int fd, short Event)
{
    if (m_Closed)
        return;

    m_Closed = true;

    LOG(INFO) << "fd: " << m_Socket->GetSocket() << " OnClose";

    InvokeService(REQUEST_CLOSE_PIPE, nullptr);
}