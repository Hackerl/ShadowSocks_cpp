//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_INODE_H
#define SHADOWSOCKSR_CPP_INODE_H

#include <cstddef>
#include "Common/Interface.h"

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

template <class L, class N1, class N2, class R>
inline bool NodeConnect(L * X, N1 * Y, N2 * M, R * N)
{
    auto NodeX = dynamic_cast<INode *>(X);
    auto NodeY = dynamic_cast<INode *>(Y);

    auto NodeM = dynamic_cast<INode *>(M);
    auto NodeN = dynamic_cast<INode *>(N);

    if (NodeX == nullptr || NodeY == nullptr || NodeM == nullptr || NodeN == nullptr)
        return false;

    NodeX->SetUpNode(NodeY);

    NodeY->SetDownNode(NodeX);
    NodeY->SetUpNode(NodeM);

    NodeM->SetDownNode(NodeY);
    NodeM->SetUpNode(NodeN);

    NodeN->SetDownNode(NodeM);

    return true;
}

#endif //SHADOWSOCKSR_CPP_INODE_H
