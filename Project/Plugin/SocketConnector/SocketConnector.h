//
// Created by patteliu on 2018/10/26.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
#define SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H

#include <Node/Node.h>
#include <Plugin/IPlugin.h>
#include <Plugin/CommonProxy/CommonProtocol.h>

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

class CSocketConnector : public IPlugin, public CNode, public INodeService
{
public:
    CSocketConnector();

public:
    bool NodeInit(std::shared_ptr<INodeManager> NodeManager) override;

public:
    bool OnNodeService(NodeServiceRegister ServiceID, void *Context) override;

public:
    bool SetConfig(const Json::Value &Config) override;

public:
    bool NoProxyHandler(CConnectRequest * ConnectInfo);
    bool HTTPTunnelHandler(CConnectRequest * ConnectInfo);

private:
    CConnectorConfig m_Config;
};

#endif //SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
