//
// Created by patteliu on 2018/10/26.
//

#include "SocketConnector.h"
#include "Socket/LibSocketExport.h"
#include "Common/JSONHelper.h"

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

bool CSocketConnector::OnNodeInit(void *arg)
{
    //TODO parse target ip and port, connect with proxy config, init remote socket node

    auto ConnectInfo = static_cast<CConnectRequest *>(arg);

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

    return InitUpNode(Socket);
}

bool CSocketConnector::HTTPTunnelHandler(CConnectRequest * ConnectInfo)
{
    return false;
}
