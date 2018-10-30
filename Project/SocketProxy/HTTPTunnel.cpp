//
// Created by hackerl on 10/29/18.
//

#include "HTTPTunnel.h"
#include <sstream>
#include <cstring>

#define HTTP_LINE_SPLIT "\r\n"
#define HTTP_HEADER_END "\r\n\r\n"

CHTTPTunnel::CHTTPTunnel()
{
    m_IsValid = false;
    m_Socket = nullptr;
    m_ProxyPort = 0;
}

CHTTPTunnel::~CHTTPTunnel()
{
    if (m_Socket != nullptr)
    {
        m_Socket->Close();
        delete m_Socket;
        m_Socket = nullptr;
    }
}

int CHTTPTunnel::GetSocket()
{
    if (m_Socket == nullptr)
        return -1;

    return m_Socket->GetSocket();
}

int CHTTPTunnel::GetSockOpt(int Level, int OptName, void *OptVal, socklen_t *OptLen)
{
    if (m_Socket == nullptr)
        return -1;

    return m_Socket->GetSockOpt(Level, OptName, OptVal, OptLen);
}

int CHTTPTunnel::SetSockOpt(int Level, int OptName, const void *OptVal, socklen_t OptLen)
{
    if (m_Socket == nullptr)
        return -1;

    return m_Socket->SetSockOpt(Level, OptName, OptVal, OptLen);
}

ssize_t CHTTPTunnel::Recv(void *Buffer, size_t Length, int Flag)
{
    if (m_Socket == nullptr)
        return -1;

    return m_Socket->Recv(Buffer, Length, Flag);
}

ssize_t CHTTPTunnel::Send(const void *Buffer, size_t Length, int Flag)
{
    if (m_Socket == nullptr)
        return -1;

    return m_Socket->Send(Buffer, Length, Flag);
}

bool CHTTPTunnel::Close()
{
    if (m_Socket == nullptr)
        return false;

    return m_Socket->Close();
}

void CHTTPTunnel::SetSocket(ITCPSocket *Socket)
{
    m_Socket = Socket;
}

void CHTTPTunnel::SetProxy(std::string IP, u_short Port)
{
    m_ProxyServer = IP;
    m_ProxyPort = Port;

    m_IsValid = true;
}

bool CHTTPTunnel::Connect(std::string IP, ushort Port)
{
    if (!m_IsValid || m_Socket == nullptr)
        return false;

    if (!m_Socket->Connect(m_ProxyServer, m_ProxyPort))
        return false;

    std::ostringstream TunnelRequest;

    TunnelRequest << "CONNECT "<< IP << ":" << Port <<" HTTP/1.1" << HTTP_LINE_SPLIT;
    TunnelRequest << "HOST: " << IP << ":" << Port << HTTP_LINE_SPLIT;
    TunnelRequest << "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:40.0) Gecko/20100101 Firefox/44.0" << HTTP_LINE_SPLIT;
    TunnelRequest << "Proxy-Connection: keep-alive" << HTTP_HEADER_END;

    std::string Data = TunnelRequest.str();

    if (m_Socket->Send(Data.data(), Data.size()) <= 0)
        return false;

    u_char Buffer[1024] = {};

    ssize_t ReadLen = m_Socket->Recv(Buffer, sizeof(Buffer), 0);

    if (ReadLen <= 0)
        return false;

    bool Success = false;

    for (int i = 0; i < ReadLen; i++)
    {
        if (Buffer[i] != ' ')
            continue;

        if (i + 4 > ReadLen)
            break;

        if (!memcmp(Buffer + i, " 200", 4))
        {
            Success = true;
            break;
        }
    }

    return Success;
}
