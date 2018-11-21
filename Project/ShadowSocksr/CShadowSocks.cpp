//
// Created by patteliu on 2018/11/20.
//

#include "CShadowSocks.h"
#include <EventLoop/LibEventExport.h>
#include <Socket/LibSocketExport.h>
#include <glog/logging.h>
#include <SocketNode/LibSocketNodeExport.h>
#include <Plugin/IPlugin.h>
#include <Plugin/LibPluginExport.h>
#include <Node/NodeManager.h>
#include <Common/JSONHelper.h>

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

    for (auto const & PluginLoader : m_PluginLoaderList)
        delete PluginLoader;

    m_PluginLoaderList.clear();
}

bool CShadowSocks::SetConfig(Json::Value & Config)
{
    std::string Server = g_JSON->GetString(Config, "Server");
    u_int Port = g_JSON->GetUInt(Config, "Port");

    if (Server.empty() || Port == 0)
        return false;

    m_Config.IP = Server;
    m_Config.Port = u_short(Port);

    if (!g_JSON->HasArray(Config, "Plugins"))
        return false;

    Json::Value Plugins = Config["Plugins"];

    for (auto const & Plugin : Plugins)
    {
        CPluginLoader * PluginLoader = new CPluginLoader;

        std::string Name = g_JSON->GetString(Plugin, "Name");

        PluginLoader->Init(Name.c_str());

        if (g_JSON->HasObject(Plugin, "Config"))
            PluginLoader->SetConfig(Plugin["Config"]);

        m_PluginLoaderList.push_back(PluginLoader);
    }

    return true;
}

void CShadowSocks::OnRead(int fd, short Event)
{
    CCNodeManager * NodeMgr = new InstanceManager<CCNodeManager>;

    ITCPSocket * Local = m_Socket->Accept();

    ISocketNode * LocalNode = NewLocalSocketNode();
    LocalNode->SocketNodeInit(m_EventLoop, Local);

    NodeMgr->AddNode(LocalNode);

    for (auto const & PluginLoader : m_PluginLoaderList)
    {
        IPlugin * Plugin = PluginLoader->Builder();

        if (!Plugin)
        {
            LOG(ERROR) << "Build Plugin Failed";
            continue;
        }

        NodeMgr->AddNode(Plugin);
    }

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
