//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_SOCKSSERVER_H
#define SHADOWSOCKSR_CPP_SOCKSSERVER_H

#include <Node/Node.h>
#include "Plugin/Plugin.h"
#include "Plugin/CommonProxy/CommonProtocol.h"

class CProxyServer : public IPlugin, public CNode
{
public:
    CProxyServer();

public:
    bool SetConfig(Json::Value &Config) override;

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;

public:
    bool SocksProxyHandler(Socks5_Connect_Request * Request, size_t Length);

private:
    bool m_HasInit;
};

#endif //SHADOWSOCKSR_CPP_SOCKSSERVER_H
