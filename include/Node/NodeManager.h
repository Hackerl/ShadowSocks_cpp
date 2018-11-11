//
// Created by hackerl on 11/11/18.
//

#ifndef SHADOWSOCKSR_CPP_NODEMANAGER_H
#define SHADOWSOCKSR_CPP_NODEMANAGER_H

#include "INode.h"
#include "INodeManager.h"
#include <map>

struct CNodeEventInfo
{
    unsigned int EventID;
    INodeEvent * Node;
};

class CCNodeManager : public INodeManager
{
public:
    void RegisterEvent(unsigned int EventID, INodeEvent * Node) override
    {
        CNodeEventInfo NodeEventInfo = {};

        NodeEventInfo.EventID = EventID;
        NodeEventInfo.Node = Node;

        m_NodeEventList.push_back(NodeEventInfo);
    }

    void BroadcastEvent(unsigned int EventID, void * Context) override
    {
        for (auto const & Iterator : m_NodeEventList)
        {
            if (Iterator.EventID != EventID)
                continue;

            Iterator.Node->OnNodeEvent(EventID, Context);
        }
    }

public:
    bool RegisterService(unsigned int ServiceID, INodeService * Node) override
    {
        //TODO Check Service Exist
        m_NodeServiceList.insert(std::make_pair(ServiceID, Node));
    }

    bool InvokeService(unsigned int ServiceID, void * Context)
    {
        auto Iterator = m_NodeServiceList.find(ServiceID);

        if(Iterator == m_NodeServiceList.end())
            return false;

        return Iterator->second->OnNodeService(ServiceID, Context);
    }

public:
    void AddNode(INode * Node)
    {
        m_NodeList.push_back(Node);
        Node->NodeInit(this);
    }

    void AddNode(Interface * I)
    {
        auto Node = dynamic_cast<INode *>(I);

        if (Node != nullptr)
        {
            m_NodeList.push_back(Node);
            Node->NodeInit(this);
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
    std::map<unsigned int, INodeService *> m_NodeServiceList;
};

#endif //SHADOWSOCKSR_CPP_NODEMANAGER_H
