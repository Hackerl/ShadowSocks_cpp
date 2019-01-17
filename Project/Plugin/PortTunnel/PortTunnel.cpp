//
// Created by patteliu on 2019/1/17.
//

#include "PortTunnel.h"
#include <Plugin/CommonProxy/CommonProtocol.h>

CPortTunnel::CPortTunnel() : m_Config()
{
    m_HasInit = false;
    m_IsValid = false;
}

bool CPortTunnel::InitPlugin(const void *Context, size_t Size)
{
    Json::Value Config;
    std::string Error;

    if (!ParseJsonFromData((const char *)Context, Size, Config, Error))
        return false;

    u_int Port = g_JSON->GetUInt(Config, "TargetPort", 0x10000);
    std::string IP = g_JSON->GetString(Config, "TargetIP");

    if (Port > 0xffff || IP.empty())
        return false;

    m_Config.TargetIP = IP;
    m_Config.TargetPort = (u_short) Port;

    m_IsValid = true;

    return true;
}

bool CPortTunnel::OnUpStream(const void *Buffer, size_t Length)
{
    if (!m_IsValid)
        return false;

    if (!m_HasInit)
    {
        m_HasInit = true;

        CConnectRequest ConnectRequest = {};

        ConnectRequest.SocketAddress.Header.AddressType = IPv4Type;
        ConnectRequest.SocketAddress.Header.SocketType = TCPSocketType;

        ConnectRequest.SocketAddress.Address = m_Config.TargetIP;
        ConnectRequest.SocketAddress.Port = m_Config.TargetPort;

        if (!InvokeService(REQUEST_SOCKET_CONNECT, &ConnectRequest) || InvokeService(INIT_REMOTE_SOCKET, ConnectRequest.Result))
            return false;
    }

    return UpStream(Buffer, Length);
}