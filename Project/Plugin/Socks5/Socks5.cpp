//
// Created by patteliu on 2018/10/24.
//

#include "Socks5.h"
#include "Plugin/CommonProxy/CommonProtocol.h"

CSocks5::CSocks5() {
    m_Status = MethodRequestStage;
}

bool CSocks5::OnPipeIn(const void *Buffer, size_t Length) {

    bool Res = false;

    switch (m_Status)
    {
        case MethodRequestStage:
            break;

        case ConnectRequestStage:
            break;

        case ConnectSuccessStage:
            Res = CPlugin::OnPipeIn(Buffer, Length);
            break;
    }

    return Res;
}

bool CSocks5::MethodRequestHandler(const void *Buffer, size_t Length)
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

        return Reply(&MethodResponse, sizeof(MethodResponse));

    } while (false);

    Destroy();

    return false;
}

bool CSocks5::ConnectRequestHandler(const void *Buffer, size_t Length) {
    do
    {
        if (Length < sizeof(Socks5_Connect_Request))
            break;

        auto ConnectRequest = (Socks5_Connect_Request *)Buffer;

        if (ConnectRequest->Command != SOCKS5_CONNECT_COMMAND)
            break;

        if (ConnectRequest->AddressType != SocksIPv4Type &&
            ConnectRequest->AddressType != SocksIPv6Type &&
            ConnectRequest->AddressType != SocksHostNameType)
            break;

        //auto * Address = (Socks5_Address *)ConnectRequest->Address;


    } while (false);

    Destroy();

    return false;
}
