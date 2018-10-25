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

bool CommonProxy::SetConfig(Json::Value &Config)
{
    return true;
}

bool CommonProxy::OnUpStream(const void *Buffer, size_t Length)
{
    if (m_HasInit)
        return UpStream(Buffer, Length);

    m_HasInit = true;

    return CommonProxyHandler((CCommonProxyRequest *) Buffer, Length);;
}

bool CommonProxy::CommonProxyHandler(CCommonProxyRequest *ProxyRequest, size_t Length)
{
    if (Length < sizeof(ProxyRequest->Header) + sizeof(ProxyRequest->IPv4Address))
        return false;

    ITCPSocket * Socket = NewTCPSocket();

    if (!Socket->Connect(ProxyRequest->IPv4Address, ProxyRequest->Port))
    {
        Socket->Close();
        delete Socket;

        return false;
    }

    InitUpNode(Socket);

    return true;
}
