//
// Created by patteliu on 2018/10/24.
//

#include "CommonProxy.h"

#include <Socket/LibSocketExport.h>
#include <Plugin/CommonProxy/CommonProtocol.h>
#include <Plugin/Socks5/Socks5Protocol.h>

CommonProxy::CommonProxy()
{
    m_HasInit = false;
}

bool CommonProxy::OnPipeIn(const void *Buffer, size_t Length)
{
    if (m_HasInit)
        return m_PipeNode->PipeOut(Buffer, Length);

    m_HasInit = true;

    bool Res = false;

    auto ProxyRequest = (CCommonProxyRequest *) Buffer;

    switch (ProxyRequest->Header.ProxyType)
    {
        case Socks5ProxyType:
            Res = Socks5ProxyHandler(ProxyRequest, Length);
            break;

        default:
            Destroy();
    }

    return Res;
}

bool CommonProxy::Socks5ProxyHandler(CCommonProxyRequest *ProxyRequest, size_t Length)
{
    Socks5_Connect_Response Response = {};

    Response.Header.Version = SOCKS5_VERSION;
    Response.Header.Response = 0x00;
    Response.Header.Reserved = 0x00;
    Response.Header.AddressType = SocksIPv4Type;
    Response.Address.IPv4.IP = 0x00;
    Response.Address.IPv4.Port = 0x80;

    switch (ProxyRequest->Header.AddressType)
    {
        case IPv4Type:
            if (Length < sizeof(ProxyRequest->Header) + sizeof(ProxyRequest->IPv4Address))
                break;

            ITCPSocket * Socket = NewTCPSocket();

            if (!Socket->Connect(ProxyRequest->IPv4Address, ProxyRequest->Port))
            {
                Socket->Close();
                delete Socket;

                Response.Header.Response = 0x01;
                m_PipeNode->PipeIn(&Response, sizeof(Response));

                break;
            }

            NodeInit(Socket);

            m_PipeNode->PipeIn(&Response, sizeof(Response));

            break;
    }

    return true;
}
