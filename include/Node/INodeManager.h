//
// Created by hackerl on 11/11/18.
//

#ifndef SHADOWSOCKSR_CPP_INODEMANAGER_H
#define SHADOWSOCKSR_CPP_INODEMANAGER_H

#include "NodeEvent.h"
#include "NodeService.h"
#include <Common/Interface.h>

class INodeEvent : public Interface
{
public:
    virtual void OnNodeEvent(NodeEventRegister EventID, void *Context) = 0;
};

class INodeService : public Interface
{
public:
    virtual bool OnNodeService(NodeServiceRegister ServiceID, void *Context) = 0;
};

class INodeManager : public Interface
{
public:
    virtual void RegisterEvent(NodeEventRegister EventID, INodeEvent *Node) = 0;
    virtual void BroadcastEvent(NodeEventRegister EventID, void *Context, INodeEvent *Publisher) = 0;
    virtual bool RegisterService(NodeServiceRegister ServiceID, INodeService *Node) = 0;
    virtual bool InvokeService(NodeServiceRegister ServiceID, void *Context) = 0;
};

#endif //SHADOWSOCKSR_CPP_INODEMANAGER_H
