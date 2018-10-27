//
// Created by patteliu on 2018/10/24.
//

#include "SocksServer.h"

#include <Socket/LibSocketExport.h>
#include <Plugin/CommonProxy/CommonProtocol.h>
#include <Plugin/Socks5/Socks5Protocol.h>

CProxyServer::CProxyServer()
{
    m_HasInit = false;
}

bool CProxyServer::SetConfig(Json::Value &Config)
{
    return true;
}

bool CProxyServer::OnUpStream(const void *Buffer, size_t Length)
{
    if (m_HasInit)
        return UpStream(Buffer, Length);

    m_HasInit = true;

    return SocksProxyHandler((CConnectRequest *) Buffer, Length);
}

bool CProxyServer::SocksProxyHandler(CConnectRequest *ProxyRequest, size_t Length)
{
    if (Length < sizeof(ProxyRequest->Header) + sizeof(ProxyRequest->IPv4Address))
        return false;

    Socks5_Connect_Response Response = {};

    if (InitUpNode(ProxyRequest))
        Response.Header.Response = uint8_t(0x00);

    return DownStream(&Response, sizeof(Socks5_Connect_Response));
}
