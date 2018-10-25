//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_INODE_H
#define SHADOWSOCKSR_CPP_INODE_H

#include <cstddef>
#include "Common/Interface.h"

class INodeCallback : public Interface
{
public:
    virtual bool OnUpStream(const void *Buffer, size_t Length) = 0;
    virtual bool OnDownStream(const void *Buffer, size_t Length) = 0;
};

class INode : public Interface
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

#endif //SHADOWSOCKSR_CPP_INODE_H
