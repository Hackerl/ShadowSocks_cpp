//
// Created by patteliu on 2018/10/26.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
#define SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H

#include <Node/Node.h>
#include <Plugin/IPlugin.h>
#include <Plugin/CommonProxy/CommonProtocol.h>
#include <Socket/LibSocketExport.h>

enum CSocketProxyType
{
    NOProxyType = 0,
    HTTPTunnelType,
    Socks5ProxyType
};

struct CConnectorConfig
{
    CSocketProxyType ProxyType;
    std::string ProxyServer;
    u_short ProxyPort;
};

class CSocketConnector : public IPlugin, public INodeManagerRegister, public INodeServiceCallback
{
public:
    CSocketConnector();

public:
    bool OnInitManager(INodeManager *NodeManager) override;

public:
    bool OnNodeService(NodeServiceRegister ServiceID, void *Context) override;

public:
    bool InitPlugin(const void *Context, size_t Size) override;

public:
    bool NoProxyHandler(CConnectRequest *ConnectRequest);
    bool HTTPTunnelHandler(CConnectRequest *ConnectRequest);

private:
    CConnectorConfig m_Config;
};

#endif //SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
