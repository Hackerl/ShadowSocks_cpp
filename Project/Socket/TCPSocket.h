//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_TCPSOCKET_H
#define SHADOWSOCKSR_CPP_TCPSOCKET_H

#include "Socket/ISocket.h"

class CTCPSocket : public ITCPSocket
{
public:
    CTCPSocket();
    ~CTCPSocket();

public:
    int GetSocket();

public:
    bool Bind(std::string IP, ushort Port) override;
    bool Listen(int backlog) override;
    ssize_t Recv(char * Buffer, size_t Length, int Flag) override;
    ssize_t Send(const char * Buffer, size_t Length, int Flag) override;
    bool Connect(std::string IP, ushort Port) override;
    int Accpet() override;
    bool Close() override;

private:
    int m_Socket;
    bool m_IsValid;
    bool m_IsConnected;
};


#endif //SHADOWSOCKSR_CPP_TCPSOCKET_H
