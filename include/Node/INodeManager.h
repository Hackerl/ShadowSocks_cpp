//
// Created by hackerl on 11/11/18.
//

#ifndef SHADOWSOCKSR_CPP_INODEMANAGER_H
#define SHADOWSOCKSR_CPP_INODEMANAGER_H

#include "Common/Interface.h"

class INodeEvent : public Interface
{
public:
    virtual void OnNodeEvent(unsigned int EventID, void * Context) = 0;
};

class INodeService : public Interface
{
public:
    virtual bool OnNodeService(unsigned int ServiceID, void * Context) = 0;
};

class INodeManager : public Interface
{
public:
    virtual void RegisterEvent(unsigned int EventID, INodeEvent * Node) = 0;
    virtual void BroadcastEvent(unsigned int EventID, void * Context) = 0;
    virtual bool RegisterService(unsigned int ServiceID, INodeService * Node) = 0;
    virtual bool InvokeService(unsigned int ServiceID, void * Context) = 0;
};

#endif //SHADOWSOCKSR_CPP_INODEMANAGER_H
