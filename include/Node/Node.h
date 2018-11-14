//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_NODE_H
#define SHADOWSOCKSR_CPP_NODE_H

#include "INode.h"
#include "Node/NodeEvent.h"
#include "Common/IInstanceManager.h"

class CNode : public INode
{
public:
    CNode()
    {
        m_UpNode = nullptr;
        m_DownNode = nullptr;
        m_NodeManager = nullptr;
        m_NodeClosed = false;
    }

public:
    bool NodeInit(INodeManager * NodeManager) override
    {
        m_NodeManager = NodeManager;
        AddRef(NodeManager);
    }

    void NodeClose() override
    {
        if (m_NodeClosed)
            return;

        m_NodeClosed = true;

        //TODO Node Close

        m_UpNode = nullptr;
        m_DownNode = nullptr;

        if (m_NodeManager != nullptr)
        {
            Release(m_NodeManager);
            m_NodeManager = nullptr;
        }
    }

public:
    bool UpStream(const void *Buffer, size_t Length) override
    {
        if (m_UpNode == nullptr)
            return false;

        return m_UpNode->OnUpStream(Buffer, Length);
    }

    bool DownStream(const void *Buffer, size_t Length) override
    {
        if (m_DownNode == nullptr)
            return false;

        return m_DownNode->OnDownStream(Buffer, Length);

    }

    void SetUpNode(INodeCallback * UpNode) override
    {
        m_UpNode = UpNode;
    }

    void SetDownNode(INodeCallback * DownNode) override
    {
        m_DownNode = DownNode;
    }

public:
    bool OnUpStream(const void *Buffer, size_t Length) override
    {
        return UpStream(Buffer, Length);
    }

    bool OnDownStream(const void *Buffer, size_t Length) override
    {
        return DownStream(Buffer, Length);
    }

public:
    bool RegisterEvent(NodeEventRegister EventID, INodeEvent * Node)
    {
        if (m_NodeManager == nullptr)
            return false;

        m_NodeManager->RegisterEvent(EventID, Node);
    }

    bool BroadcastEvent(NodeEventRegister EventID, void * Context, INodeEvent * Publisher = nullptr)
    {
        if (m_NodeManager == nullptr)
            return false;

        m_NodeManager->BroadcastEvent(EventID, Context, Publisher);
    }

    bool RegisterService(NodeServiceRegister ServiceID, INodeService * Node)
    {
        if (m_NodeManager == nullptr)
            return false;

        return m_NodeManager->RegisterService(ServiceID, Node);
    }

    bool InvokeService(NodeServiceRegister ServiceID, void * Context)
    {
        if (m_NodeManager == nullptr)
            return false;

        return m_NodeManager->InvokeService(ServiceID, Context);
    }

private:
    bool m_NodeClosed;
    INodeCallback * m_UpNode;
    INodeCallback * m_DownNode;
    INodeManager * m_NodeManager;
};

#endif //SHADOWSOCKSR_CPP_NODE_H
