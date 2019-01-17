//
// Created by hackerl on 11/11/18.
//

#ifndef SHADOWSOCKSR_CPP_INODEMANAGER_H
#define SHADOWSOCKSR_CPP_INODEMANAGER_H

#include "NodeEventDef.h"
#include "NodeServiceDef.h"
#include <Common/Interface.h>

class INodeEventCallback : public Interface
{
public:
    virtual void OnNodeEvent(NodeEventRegister EventID, void *Context) = 0;
};

class INodeServiceCallback : public Interface
{
public:
    virtual bool OnNodeService(NodeServiceRegister ServiceID, void *Context) = 0;
};

class INodeManager : public Interface
{
public:
    virtual void RegisterEvent(NodeEventRegister EventID, INodeEventCallback *Node) = 0;
    virtual void BroadcastEvent(NodeEventRegister EventID, void *Context, INodeEventCallback *Publisher) = 0;
    virtual bool RegisterService(NodeServiceRegister ServiceID, INodeServiceCallback *Node) = 0;
    virtual bool InvokeService(NodeServiceRegister ServiceID, void *Context) = 0;
};

class INodeManagerRegister : public Interface
{
public:
    virtual bool OnInitManager(INodeManager *NodeManager) = 0;
};

#endif //SHADOWSOCKSR_CPP_INODEMANAGER_H
