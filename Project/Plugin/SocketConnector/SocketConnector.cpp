//
// Created by patteliu on 2018/10/26.
//

#include "SocketConnector.h"
#include <Common/JSONHelper.h>
#include <Node/NodeServiceDef.h>
#include <Socket/HTTPTunnel.h>

CSocketConnector::CSocketConnector() : m_Config()
{
    m_Config.ProxyType = NOProxyType;
}

bool CSocketConnector::InitPlugin(const void *Context, size_t Size)
{
    Json::Value Config;
    std::string Error;

    if (!ParseJsonFromData((const char *)Context, Size, Config, Error))
        return false;

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

bool CSocketConnector::OnInitManager(INodeManager *NodeManager)
{
    return NodeManager->RegisterService(REQUEST_SOCKET_CONNECT, this);
}

bool CSocketConnector::OnNodeService(NodeServiceRegister ServiceID, void *Context)
{
    if (ServiceID != REQUEST_SOCKET_CONNECT)
        return false;

    auto ConnectRequest = static_cast<CConnectRequest *>(Context);

    bool Res = false;

    switch (m_Config.ProxyType)
    {
        case NOProxyType:
            Res = NoProxyHandler(ConnectRequest);
            break;

        case HTTPTunnelType:
            Res = HTTPTunnelHandler(ConnectRequest);
            break;
    }

    return Res;
}

bool CSocketConnector::NoProxyHandler(CConnectRequest *ConnectRequest)
{
    if (ConnectRequest->SocketAddress.Header.AddressType != IPv4Type || ConnectRequest->SocketAddress.Header.SocketType != TCPSocketType)
        return false;

    ITCPSocket * Socket = NewTCPSocket();

    if (!Socket->Connect(ConnectRequest->SocketAddress.Address, ConnectRequest->SocketAddress.Port))
    {
        Socket->Close();
        delete Socket;

        return false;
    }

    ConnectRequest->Result = Socket;

    return true;
}

bool CSocketConnector::HTTPTunnelHandler(CConnectRequest *ConnectRequest)
{
    ITCPSocket * Socket = NewTCPSocket();

    CHTTPTunnel HTTPTunnel;

    HTTPTunnel.SetProxy(m_Config.ProxyServer, m_Config.ProxyPort);

    if (!HTTPTunnel.Connect(Socket, ConnectRequest->SocketAddress.Address, ConnectRequest->SocketAddress.Port))
    {
        Socket->Close();
        delete Socket;

        return false;
    }

    ConnectRequest->Result = Socket;

    return true;
}
