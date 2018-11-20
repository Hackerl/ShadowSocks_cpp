//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_TCPSOCKET_H
#define SHADOWSOCKSR_CPP_TCPSOCKET_H

#include <Socket/ISocket.h>

class CTCPSocket : public ITCPSocket
{
public:
    CTCPSocket();
    explicit CTCPSocket(int fd, bool IsConnected = true);
    ~CTCPSocket();

public:
    int GetSocket() override;

public:
    bool SetSendTimeOut(time_t TimeOut);
    bool SetRecvTimeOut(time_t TimeOut);

public:
    bool Bind(std::string IP, ushort Port) override;
    bool Listen(int backlog) override;
    bool Connect(std::string IP, ushort Port, time_t TimeOut) override;
    bool Connect(in_addr_t IP, in_port_t Port, time_t TimeOut) override;
    ITCPSocket * Accept() override;

public:
    bool Close() override;
    ssize_t Recv(void *Buffer, size_t Length, int Flag) override;
    ssize_t Send(const void *Buffer, size_t Length, int Flag) override;
    int GetSockOpt(int Level, int OptName, void * OptVal, socklen_t * OptLen) override;
    int SetSockOpt(int Level, int OptName, const void * OptVal, socklen_t OptLen) override;

private:
    int m_Socket;
    bool m_IsValid;
    bool m_IsConnected;
};

#endif //SHADOWSOCKSR_CPP_TCPSOCKET_H
