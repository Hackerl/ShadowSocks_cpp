//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_SOCKSSERVER_H
#define SHADOWSOCKSR_CPP_SOCKSSERVER_H

#include <Node/Node.h>
#include <jsoncpp/json/json.h>
#include <Plugin/CommonProxy/CommonProtocol.h>

class CProxyServer : public CNode
{
public:
    CProxyServer();

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;

public:
    bool SocksProxyHandler(Socks5_Connect_Request * Request, size_t Length);

private:
    bool m_HasInit;
};

#endif //SHADOWSOCKSR_CPP_SOCKSSERVER_H
