//
// Created by patteliu on 2018/10/26.
//

#include <Plugin/CommonProxy/CommonProtocol.h>
#include "SocketConnector.h"
#include "Plugin/CommonProxy/CommonProtocol.h"
#include "Socket/LibSocketExport.h"

CSocketConnector::CSocketConnector() : m_Config()
{
    m_Config.ProxyType = NOProxyType;
}

bool CSocketConnector::SetConfig(Json::Value &Config)
{
    //TODO parse config
    return true;
}

bool CSocketConnector::OnNodeInit(void *arg)
{
    //TODO parse target ip and port, connect with proxy config, init remote socket node

    auto ConnectInfo = static_cast<CConnectRequest *>(arg);

    if (ConnectInfo->Header.AddressType != IPv4Type || ConnectInfo->Header.SocketType != TCPSocketType)
        return false;

    ITCPSocket * Socket = NewTCPSocket();

    if (!Socket->Connect(ConnectInfo->IPv4Address, ConnectInfo->Port))
    {
        Socket->Close();
        delete Socket;

        return false;
    }

    return InitUpNode(Socket);
}
