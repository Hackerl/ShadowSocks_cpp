//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_REMOTESOCKETNODE_H
#define SHADOWSOCKSR_CPP_REMOTESOCKETNODE_H

#include "SocketNode.h"

class CRemoteSocketNode : public CSocketNode, public INodeInit
{
public:
    bool OnNodeInit(void * arg) override
    {
        if (m_Socket != nullptr)
            return false;

        m_Socket = static_cast<IIOSocket *>(arg);

        if (m_Loop != nullptr && m_Socket != nullptr)
            return m_Loop->AddClient(m_Socket->GetSocket(), this);
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
