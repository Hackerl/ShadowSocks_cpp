//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_REMOTESOCKETNODE_H
#define SHADOWSOCKSR_CPP_REMOTESOCKETNODE_H

#include "SocketNode.h"
#include <Node/NodeService.h>
#include <netinet/tcp.h>

class CRemoteSocketNode : public CSocketNode, public INodeService
{
public:
    bool NodeInit(std::shared_ptr<INodeManager> NodeManager) override
    {
        CSocketNode::NodeInit(NodeManager);

        return RegisterService(INIT_REMOTE_SOCKET, this);
    }

public:
    bool OnNodeService(NodeServiceRegister ServiceID, void *Context) override
    {
        if (ServiceID != INIT_REMOTE_SOCKET)
            return false;

        if (m_Socket != nullptr)
            return false;

        m_Socket = static_cast<IIOSocket *>(Context);

        if (m_Loop == nullptr || m_Socket == nullptr)
            return false;

        int OptVal = 1;

        m_Socket->SetSockOpt(IPPROTO_TCP, TCP_NODELAY, &OptVal, sizeof(OptVal));

        return m_Loop->Add(m_Socket->GetSocket(), this);
    }

public:
    bool DataIn(const void *Buffer, size_t Length) override
    {
        return DownStream(Buffer, Length);
    }

    bool OnUpStream(const void *Buffer, size_t Length) override
    {
        return DataOut(Buffer, Length);
    }
};

#endif //SHADOWSOCKSR_CPP_REMOTESOCKETNODE_H
