//
// Created by patteliu on 2018/11/20.
//

#include "ShadowSocks.h"
#include <glog/logging.h>
#include <Plugin/IPlugin.h>
#include <Node/NodeManager.h>
#include <Common/JSONHelper.h>
#include <Socket/LibSocketExport.h>
#include <EventLoop/LibEventExport.h>
#include <SocketNode/LibSocketNodeExport.h>

CShadowSocks::CShadowSocks()
{
    m_Socket = NewTCPSocket();
    m_EventLoop = NewEventLoop();
}

CShadowSocks::~CShadowSocks()
{
    delete m_EventLoop;
    m_EventLoop = nullptr;

    delete m_Socket;
    m_Socket = nullptr;

    for (auto const & Service : m_ServiceList)
        delete Service;

    m_ServiceList.clear();
}

bool CShadowSocks::SetConfig(Json::Value & Config)
{
    std::string Server = g_JSON->GetString(Config, "Server");
    u_int Port = g_JSON->GetUInt(Config, "Port");

    if (Server.empty() || Port == 0)
    {
        LOG(ERROR) << "Server Config Invalid";
        return false;
    }

    m_Config.IP = Server;
    m_Config.Port = u_short(Port);

    if (!g_JSON->HasArray(Config, "Nodes"))
    {
        LOG(ERROR) << "Plugins Config Invalid";
        return false;
    }

    Json::Value Nodes = Config["Nodes"];

    for (auto const & Node : Nodes)
    {
        std::string Name = g_JSON->GetString(Node, "Name");

        g_PluginLoader->Add(Name.c_str(), Node["Config"]);

        m_NodeNameList.push_back(Name);
    }

    Json::Value Services = Config["Services"];

    for (auto const & Service : Services)
    {
        std::string Name = g_JSON->GetString(Service, "Name");

        g_PluginLoader->Add(Name.c_str(), Service["Config"]);

        IPlugin * Plugin = g_PluginLoader->Builder(Name.c_str());

        if (!Plugin)
        {
            LOG(ERROR) << "Build Plugin Failed";
            continue;
        }

        m_ServiceList.push_back(Plugin);
    }

    return true;
}

void CShadowSocks::OnRead(int fd, short Event)
{
    auto NodeMgr = new CCNodeManager;

    ITCPSocket * Local = m_Socket->Accept();

    ISocketNode * LocalNode = NewLocalSocketNode();
    LocalNode->SocketNodeInit(m_EventLoop, Local);

    NodeMgr->AddNode(LocalNode);

    for (auto const & PluginName : m_NodeNameList)
    {
        IPlugin * Plugin = g_PluginLoader->Builder(PluginName.c_str());

        if (!Plugin)
        {
            LOG(ERROR) << "Build Plugin Failed";
            continue;
        }

        NodeMgr->AddNode(Plugin);
    }

    for (auto const & Service : m_ServiceList)
        NodeMgr->AddService(Service);

    ISocketNode * RemoteNode = NewRemoteSocketNode();
    RemoteNode->SocketNodeInit(m_EventLoop);

    NodeMgr->AddNode(RemoteNode);

    NodeMgr->Connect();
}

void CShadowSocks::OnWrite(int fd, short Event)
{

}

void CShadowSocks::OnClose(int fd, short Event)
{
    m_EventLoop->Remove(m_Socket->GetSocket());
}

void CShadowSocks::Run()
{
    int OptVal = 1;

    m_Socket->SetSockOpt(SOL_SOCKET, SO_REUSEADDR, &OptVal, sizeof(OptVal));

    if (!m_Socket->Bind(m_Config.IP, m_Config.Port))
    {
        LOG(ERROR) << "Socket Bind Failed";
        return;
    }

    if (!m_Socket->Listen(255))
    {
        LOG(ERROR) << "Socket Listen Failed";
        return;
    }

    m_EventLoop->Add(m_Socket->GetSocket(), this);

    m_EventLoop->Loop();
}
