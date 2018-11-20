//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_INODE_H
#define SHADOWSOCKSR_CPP_INODE_H

#include "INodeManager.h"
#include <cstddef>

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
    virtual bool NodeInit(INodeManager * NodeManager) = 0;
    virtual bool UpStream(const void *Buffer, size_t Length) = 0;
    virtual bool DownStream(const void *Buffer, size_t Length) = 0;
    virtual void SetUpNode(INodeCallback * UpNode) = 0;
    virtual void SetDownNode(INodeCallback * DownNode) = 0;
};

#endif //SHADOWSOCKSR_CPP_INODE_H
