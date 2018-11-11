//
// Created by patteliu on 2018/10/26.
//

#include "SocketConnector.h"
#include "Socket/LibSocketExport.h"
#include "SocketProxy/LibSocketProxyExport.h"
#include "Common/JSONHelper.h"
#include "Node/NodeService.h"

CSocketConnector::CSocketConnector() : m_Config()
{
    m_Config.ProxyType = NOProxyType;
}

bool CSocketConnector::SetConfig(Json::Value &Config)
{
    //TODO parse config
    std::string Type = g_JSON->GetString(Config, "ProxyType");

    std::string Server = g_JSON->GetString(Config, "ProxyServer");
    u_int Port = g_JSON->GetUInt(Config, "ProxyPort");

    if (Type.empty() || Server.empty() || Port == 0)
        return true;

    if (Type == "Socks5")
        m_Config.ProxyType = Socks5ProxyType;

    if (Type == "HTTPTunnel")
        m_Config.ProxyType = HTTPTunnelType;

    m_Config.ProxyServer = Server;
    m_Config.ProxyPort = u_short(Port);

    return true;
}

bool CSocketConnector::NodeInit(INodeManager *NodeManager)
{
    CNode::NodeInit(NodeManager);

    return m_NodeManager->RegisterService(REQUEST_SOCKET_CONNECT, this);
}

bool CSocketConnector::OnNodeService(unsigned int ServiceID, void *Context)
{
    //TODO parse target ip and port, connect with proxy config, init remote socket node

    if (ServiceID != REQUEST_SOCKET_CONNECT)
        return false;

    auto ConnectInfo = static_cast<CConnectRequest *>(Context);

    bool Res = false;

    switch (m_Config.ProxyType)
    {
        case NOProxyType:
            Res = NoProxyHandler(ConnectInfo);
            break;

        case HTTPTunnelType:
            Res = HTTPTunnelHandler(ConnectInfo);
            break;
    }

    return Res;
}

bool CSocketConnector::NoProxyHandler(CConnectRequest * ConnectInfo)
{
    if (ConnectInfo->Header.AddressType != IPv4Type || ConnectInfo->Header.SocketType != TCPSocketType)
        return false;

    ITCPSocket * Socket = NewTCPSocket();

    if (!Socket->Connect(ConnectInfo->Address, ConnectInfo->Port))
    {
        Socket->Close();
        delete Socket;

        return false;
    }

    return m_NodeManager->InvokeService(INIT_REMOTE_SOCKET, Socket);
}

bool CSocketConnector::HTTPTunnelHandler(CConnectRequest * ConnectInfo)
{
    ITCPSocket * Socket = NewTCPSocket();

    ISocketProxy * HTTPTunnel = NewHTTPTunnel();

    HTTPTunnel->SetSocket(Socket);
    HTTPTunnel->SetProxy(m_Config.ProxyServer, m_Config.ProxyPort);

    auto HTTPTunnelSocket = dynamic_cast<IIOSocket *>(HTTPTunnel);

    if (!HTTPTunnel->Connect(ConnectInfo->Address, ConnectInfo->Port))
    {
        HTTPTunnelSocket->Close();
        delete HTTPTunnel;

        return false;
    }

    return m_NodeManager->InvokeService(INIT_REMOTE_SOCKET, HTTPTunnelSocket);
}
