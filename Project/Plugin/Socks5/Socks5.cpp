//
// Created by patteliu on 2018/10/24.
//

#include "Socks5.h"
#include "Plugin/CommonProxy/CommonProtocol.h"

CSocks5Proxy::CSocks5Proxy()
{
    m_Status = MethodRequestStage;
}

bool CSocks5Proxy::OnDataIn(const void *Buffer, size_t Length)
{
    bool Res = false;

    switch (m_Status)
    {
        case MethodRequestStage:
            MethodRequestHandler(Buffer, Length);
            break;

        case ConnectRequestStage:
            ConnectRequestHandler(Buffer, Length);
            break;

        case ConnectSuccessStage:
            Res = m_PipeNode->PipeIn(Buffer, Length);
            break;
    }

    return Res;
}

bool CSocks5Proxy::MethodRequestHandler(const void *Buffer, size_t Length)
{
    do
    {
        if (Length < sizeof(Socks5_Method_Request))
            break;

        auto MethodRequest = (Socks5_Method_Request *) Buffer;

        if (Length < MethodRequest->NMethods + sizeof(Socks5_Method_Request))
            break;

        //TO-DO Choose method

        Socks5_Method_Response MethodResponse = {};

        MethodResponse.Version = SOCKS5_VERSION;
        MethodResponse.Method = SOCKS5_NO_AUTH_METHOD;

        m_Status = ConnectRequestStage;

        return m_PipeNode->PipeOut(&MethodResponse, sizeof(MethodResponse));

    } while (false);

    Destroy();

    return false;
}

bool CSocks5Proxy::ConnectRequestHandler(const void *Buffer, size_t Length) {
    do
    {
        auto ConnectRequest = (Socks5_Connect_Request *)Buffer;

        if (Length < sizeof(ConnectRequest->Header))
            break;

        if (ConnectRequest->Header.Command != SOCKS5_CONNECT_COMMAND)
            break;

        CCommonProxyRequest ProxyRequest = ParseSocks5Address(ConnectRequest, Length);

        if (ProxyRequest.Header.AddressType == UnknownType)
            break;

        m_Status = ConnectSuccessStage;

        bool Res = m_PipeNode->PipeIn(&ProxyRequest, sizeof(CCommonProxyRequest));

        Socks5_Connect_Response Response = {};

        Response.Header.Response = Res ? uint8_t(0x00): uint8_t(0x01);

        return m_PipeNode->PipeOut(&Response, sizeof(Socks5_Connect_Response));;

    } while (false);

    Destroy();

    return false;
}
