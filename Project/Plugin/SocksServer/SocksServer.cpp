//
// Created by patteliu on 2018/10/24.
//

#include "SocksServer.h"
#include <Node/NodeServiceDef.h>
#include <Socket/LibSocketExport.h>
#include <Plugin/Socks5/Socks5Protocol.h>
#include <Plugin/PluginLoader.h>

CProxyServer::CProxyServer()
{
    m_HasInit = false;
}

bool CProxyServer::InitPlugin(const void *Context, size_t Size)
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

    CCommonSocketAddress SocketAddress = ParseSocks5Address(Request, Length);

    if (SocketAddress.Header.AddressType == UnknownType)
        return false;

    CConnectRequest ConnectRequest = {};
    Socks5_Connect_Response Response = {};

    ConnectRequest.SocketAddress = SocketAddress;

    if (InvokeService(REQUEST_SOCKET_CONNECT, &ConnectRequest) && InvokeService(INIT_REMOTE_SOCKET, ConnectRequest.Result))
        Response.Header.Response = uint8_t(0x00);

    return DownStream(&Response, sizeof(Socks5_Connect_Response));
}
