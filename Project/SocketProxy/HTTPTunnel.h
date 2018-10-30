//
// Created by hackerl on 10/29/18.
//

#ifndef SHADOWSOCKSR_CPP_HTTPTUNNEL_H
#define SHADOWSOCKSR_CPP_HTTPTUNNEL_H


#include "SocketProxy/ISocketProxy.h"

class CHTTPTunnel : public ISocketProxy, public IIOSocket
{
public:
    CHTTPTunnel();
    ~CHTTPTunnel();

public:
    int GetSocket() override;
    ssize_t Recv(void *Buffer, size_t Length, int Flag = 0) override;
    ssize_t Send(const void *Buffer, size_t Length, int Flag = 0) override;
    int GetSockOpt(int Level, int OptName, void * OptVal, socklen_t * OptLen) override;
    int SetSockOpt(int Level, int OptName, const void * OptVal, socklen_t OptLen) override;
    bool Close() override;

public:
    void SetSocket(ITCPSocket * Socket) override;
    void SetProxy(std::string IP, u_short Port) override;
    bool Connect(std::string IP, ushort Port) override;

private:
    bool m_IsValid;
    std::string m_ProxyServer;
    u_short m_ProxyPort;
    ITCPSocket * m_Socket;
};

#endif //SHADOWSOCKSR_CPP_HTTPTUNNEL_H
