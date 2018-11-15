//
// Created by patteliu on 2018/11/15.
//

#ifndef SHADOWSOCKSR_CPP_HTTPTUNNEL_H
#define SHADOWSOCKSR_CPP_HTTPTUNNEL_H

#include <sstream>
#include <cstring>
#include <Socket/ISocket.h>

#define HTTP_LINE_SPLIT "\r\n"
#define HTTP_HEADER_END "\r\n\r\n"

class CHTTPTunnel
{
public:
    CHTTPTunnel()
    {
        m_IsValid = false;
        m_ProxyPort = 0;
    }

public:
    void SetProxy(std::string IP, u_short Port)
    {
        m_ProxyServer = IP;
        m_ProxyPort = Port;

        m_IsValid = true;
    }

    bool Connect(ITCPSocket * Socket, std::string IP, ushort Port)
    {
        if (!m_IsValid)
            return false;

        if (!Socket->Connect(m_ProxyServer, m_ProxyPort))
            return false;

        std::ostringstream TunnelRequest;

        TunnelRequest << "CONNECT "<< IP << ":" << Port <<" HTTP/1.1" << HTTP_LINE_SPLIT;
        TunnelRequest << "HOST: " << IP << ":" << Port << HTTP_LINE_SPLIT;
        TunnelRequest << "User-Agent: Mozilla/5.0 (Windows NT 6.3; WOW64; rv:40.0) Gecko/20100101 Firefox/44.0" << HTTP_LINE_SPLIT;
        TunnelRequest << "Proxy-Connection: keep-alive" << HTTP_LINE_SPLIT;
        TunnelRequest << "Connection: keep-alive" << HTTP_HEADER_END;

        std::string Data = TunnelRequest.str();

        if (Socket->Send(Data.data(), Data.size()) <= 0)
            return false;

        u_char Buffer[1024] = {};

        ssize_t ReadLen = Socket->Recv(Buffer, sizeof(Buffer), 0);

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

private:
    bool m_IsValid;
    std::string m_ProxyServer;
    u_short m_ProxyPort;
};

#endif //SHADOWSOCKSR_CPP_HTTPTUNNEL_H
