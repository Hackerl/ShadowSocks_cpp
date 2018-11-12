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

        if (m_NodeManager != nullptr)
        {
            Release(m_NodeManager);
            m_NodeManager = nullptr;
        }

        if (m_UpNode != nullptr)
        {
            Release(m_UpNode);
            m_UpNode = nullptr;
        }

        if (m_DownNode != nullptr)
        {
            Release(m_DownNode);
            m_DownNode = nullptr;
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
        AddRef(UpNode);
    }

    void SetDownNode(INodeCallback * DownNode) override
    {
        m_DownNode = DownNode;
        AddRef(DownNode);
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
    void RegisterEvent(unsigned int EventID, INodeEvent * Node)
    {
        if (m_NodeManager == nullptr)
            return;

        m_NodeManager->RegisterEvent(EventID, Node);
    }

    void BroadcastEvent(unsigned int EventID, void * Context, INodeEvent * Publisher = nullptr)
    {
        if (m_NodeManager == nullptr)
            return;

        m_NodeManager->BroadcastEvent(EventID, Context, Publisher);
    }

    bool RegisterService(unsigned int ServiceID, INodeService * Node)
    {
        if (m_NodeManager == nullptr)
            return false;

        return m_NodeManager->RegisterService(ServiceID, Node);
    }

    bool InvokeService(unsigned int ServiceID, void * Context)
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
