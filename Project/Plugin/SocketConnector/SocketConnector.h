//
// Created by patteliu on 2018/10/26.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
#define SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H

#include <Node/Node.h>
#include "Plugin/Plugin.h"

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

class CSocketConnector : public IPlugin, public CNode, public INodeInit
{
public:
    CSocketConnector();

public:
    bool OnNodeInit(void * arg) override;

public:
    bool SetConfig(Json::Value &Config) override;

private:
    CConnectorConfig m_Config;
};

#endif //SHADOWSOCKSR_CPP_SOCKETCONNECTOR_H
