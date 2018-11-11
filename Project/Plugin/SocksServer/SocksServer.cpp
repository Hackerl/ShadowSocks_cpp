//
// Created by patteliu on 2018/10/24.
//

#include "SocksServer.h"
#include "Node/NodeService.h"
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

    return SocksProxyHandler((Socks5_Connect_Request *)Buffer, Length);
}

bool CProxyServer::SocksProxyHandler(Socks5_Connect_Request * Request, size_t Length)
{
    if (Length < sizeof(Request->Header))
        return false;

    CConnectRequest ProxyRequest = ParseSocks5Address(Request, Length);

    if (ProxyRequest.Header.AddressType == UnknownType)
        return false;

    Socks5_Connect_Response Response = {};

    if (m_NodeManager->InvokeService(REQUEST_SOCKET_CONNECT ,&ProxyRequest))
        Response.Header.Response = uint8_t(0x00);

    return DownStream(&Response, sizeof(Socks5_Connect_Response));
}
