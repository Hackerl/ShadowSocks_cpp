//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_INODE_H
#define SHADOWSOCKSR_CPP_INODE_H

#include <cstddef>
#include "Common/Interface.h"
#include <vector>

class INodeClose : public Interface
{
public:
    virtual void NodeClose() = 0;
};

class INodeCallback : public INodeClose
{
public:
    virtual bool OnUpStream(const void *Buffer, size_t Length) = 0;
    virtual bool OnDownStream(const void *Buffer, size_t Length) = 0;
};

class INode : public INodeCallback
{
public:
    virtual bool UpStream(const void *Buffer, size_t Length) = 0;
    virtual bool DownStream(const void *Buffer, size_t Length) = 0;
    virtual void SetUpNode(INodeCallback * UpNode) = 0;
    virtual void SetDownNode(INodeCallback * DownNode) = 0;
};

class INodeInit : public Interface
{
public:
    virtual bool OnNodeInit(void * arg) = 0;
};

class CNodeManager
{
public:
    void AddNode(INode * Node)
    {
        m_NodeList.push_back(Node);
    }

    void AddNode(Interface * I)
    {
        auto Node = dynamic_cast<INode *>(I);

        if (Node != nullptr)
            m_NodeList.push_back(Node);
    }

    void Connect()
    {
        INode * PreNode = nullptr;

        for (auto const & Node : m_NodeList)
        {
            if (PreNode != nullptr && Node != nullptr)
            {
                Node->SetDownNode(PreNode);
                PreNode->SetUpNode(Node);
            }

            PreNode = Node;
        }
    }

private:
    std::vector<INode *> m_NodeList;
};

#endif //SHADOWSOCKSR_CPP_INODE_H
