//
// Created by patteliu on 2018/10/24.
//

#include "SocksClient.h"
#include <Plugin/CommonProxy/CommonProtocol.h>

CSocks5Proxy::CSocks5Proxy()
{
    m_Status = MethodRequestStage;
}

bool CSocks5Proxy::SetConfig(Json::Value &Config) {
    return true;
}

bool CSocks5Proxy::OnUpStream(const void *Buffer, size_t Length)
{
    bool Res = false;

    switch (m_Status)
    {
        case MethodRequestStage:
            Res = MethodRequestHandler(Buffer, Length);
            break;

        case ConnectRequestStage:
            Res = ConnectRequestHandler(Buffer, Length);
            break;

        case ConnectSuccessStage:
            Res = UpStream(Buffer, Length);
            break;
    }

    return Res;
}

bool CSocks5Proxy::MethodRequestHandler(const void *Buffer, size_t Length)
{
    if (Length < sizeof(Socks5_Method_Request))
        return false;

    auto MethodRequest = (Socks5_Method_Request *) Buffer;

    if (Length < MethodRequest->NMethods + sizeof(Socks5_Method_Request))
        return false;

    //TODO Choose method

    Socks5_Method_Response MethodResponse = {};

    MethodResponse.Version = SOCKS5_VERSION;
    MethodResponse.Method = SOCKS5_NO_AUTH_METHOD;

    m_Status = ConnectRequestStage;

    return DownStream(&MethodResponse, sizeof(MethodResponse));

}

bool CSocks5Proxy::ConnectRequestHandler(const void *Buffer, size_t Length)
{
    auto ConnectRequest = (Socks5_Connect_Request *)Buffer;

    if (Length < sizeof(ConnectRequest->Header))
        return false;

    if (ConnectRequest->Header.Command != SOCKS5_CONNECT_COMMAND)
        return false;

    m_Status = ConnectSuccessStage;

    return UpStream(Buffer, Length);
}
