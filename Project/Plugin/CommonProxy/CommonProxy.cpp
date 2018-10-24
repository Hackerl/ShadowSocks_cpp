//
// Created by patteliu on 2018/10/24.
//

#include "CommonProxy.h"

#include <Socket/LibSocketExport.h>
#include <Plugin/CommonProxy/CommonProtocol.h>

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

    switch (ProxyRequest->Header.AddressType)
    {
        case Socks5ProxyType:
            Res = Socks5ProxyHandler(ProxyRequest, Length);
            break;
    }

    return Res;
}

bool CommonProxy::Socks5ProxyHandler(CCommonProxyRequest *ProxyRequest, size_t Length)
{
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

                break;
            }

            Socks5_Connect_Response Response = {};

            Response.Version = SOCKS5_VERSION;
            Response.Response = 0x00;
            Response.Reserved = 0x00;

            return m_PipeNode->PipeIn(&Response, sizeof(Response));
    }

    Destroy();

    return false;
}
