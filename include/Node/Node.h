//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_NODE_H
#define SHADOWSOCKSR_CPP_NODE_H

#include "INode.h"

class CNode : public INode, public INodeCallback
{
public:
    CNode()
    {
        m_UpNode = nullptr;
        m_DownNode = nullptr;
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
        UpStream(Buffer, Length);
    }

    bool OnDownStream(const void *Buffer, size_t Length) override
    {
        DownStream(Buffer, Length);
    }

public:
    bool InitUpNode(void * arg)
    {
        auto UpNode = dynamic_cast<INodeInit *>(m_UpNode);

        return UpNode == nullptr ? false : UpNode->OnNodeInit(arg);
    }

    bool InitDownNode(void * arg)
    {
        auto DownNode = dynamic_cast<INodeInit *>(m_DownNode);

        return DownNode == nullptr ? false : DownNode->OnNodeInit(arg);
    }

private:
    INodeCallback * m_UpNode;
    INodeCallback * m_DownNode;
};

#endif //SHADOWSOCKSR_CPP_NODE_H
