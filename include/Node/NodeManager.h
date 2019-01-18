//
// Created by hackerl on 11/11/18.
//

#ifndef SHADOWSOCKSR_CPP_NODEMANAGER_H
#define SHADOWSOCKSR_CPP_NODEMANAGER_H

#include "INode.h"
#include "INodeManager.h"
#include "NodeServiceDef.h"
#include <vector>
#include <map>

struct CNodeEventInfo
{
    unsigned int EventID;
    INodeEventCallback * Node;
};

class CCNodeManager : public INodeManager, public INodeServiceCallback
{
public:
    CCNodeManager()
    {
        RegisterService(REQUEST_CLOSE_PIPE, this);
    }

public:
    ~CCNodeManager()
    {
        m_NodeList.clear();
        m_NodeEventList.clear();
        m_NodeServiceList.clear();
    }

public:
    void RegisterEvent(NodeEventRegister EventID, INodeEventCallback *Node) override
    {
        CNodeEventInfo NodeEventInfo = {};

        NodeEventInfo.EventID = EventID;
        NodeEventInfo.Node = Node;

        m_NodeEventList.push_back(NodeEventInfo);
    }

    void BroadcastEvent(NodeEventRegister EventID, void *Context, INodeEventCallback *Publisher) override
    {
        for (auto const & it : m_NodeEventList)
        {
            if (it.EventID != EventID || Publisher == it.Node)
                continue;

            it.Node->OnNodeEvent(EventID, Context);
        }
    }

public:
    bool RegisterService(NodeServiceRegister ServiceID, INodeServiceCallback *Node) override
    {
        if (m_NodeServiceList.find(ServiceID) != m_NodeServiceList.end())
            return false;

        m_NodeServiceList.insert(std::make_pair(ServiceID, Node));

        return true;
    }

    bool InvokeService(NodeServiceRegister ServiceID, void *Context) override
    {
        auto it = m_NodeServiceList.find(ServiceID);

        if(it == m_NodeServiceList.end())
            return false;

        return it->second->OnNodeService(ServiceID, Context);
    }

public:
    bool OnNodeService(NodeServiceRegister ServiceID, void *Context) override
    {
        if (ServiceID != REQUEST_CLOSE_PIPE)
            return false;

        for (auto const & Node : m_NodeList)
        {
            Node->NodeClose();
            delete Node;
        }

        //Delete This Ptr
        delete this;

        return true;
    }

public:
    void AddService(INodeManagerRegister * Service)
    {
        Service->OnInitManager(this);
    }

    void AddService(Interface * I)
    {
        auto Service = dynamic_cast<INodeManagerRegister *>(I);

        if (Service != nullptr)
            Service->OnInitManager(this);
    }

public:
    void AddNode(INode * Node)
    {
        m_NodeList.push_back(Node);

        Node->OnInitManager(this);
    }

    void AddNode(Interface * I)
    {
        auto Node = dynamic_cast<INode *>(I);

        if (Node != nullptr)
        {
            m_NodeList.push_back(Node);

            Node->OnInitManager(this);
        }
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
    std::vector<CNodeEventInfo> m_NodeEventList;
    std::map<unsigned int, INodeServiceCallback *> m_NodeServiceList;
};

#endif //SHADOWSOCKSR_CPP_NODEMANAGER_H
