//
// Created by hackerl on 10/13/18.
//

#include "TCPSocket.h"
#include <unistd.h>
#include <arpa/inet.h>

CTCPSocket::CTCPSocket()
{
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);

    m_IsValid = m_Socket != -1;
    m_IsConnected = false;
}

CTCPSocket::~CTCPSocket()
{
    if (!m_IsValid)
        return;

    close(m_Socket);
    m_Socket = -1;
    m_IsValid = false;
}

bool CTCPSocket::Bind(std::string IP, ushort Port)
{
    if (!m_IsValid)
        return false;

    sockaddr_in Addr = {};

    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(Port);

    inet_pton(AF_INET, IP.c_str() , &Addr.sin_addr);

    int res = bind(m_Socket, (sockaddr *)&Addr, sizeof(Addr));

    return res != -1;
}

bool CTCPSocket::Listen(int backlog)
{
    if (!m_IsValid)
        return false;

    int res  = listen(m_Socket, backlog);

    return res != -1;
}

ssize_t CTCPSocket::Recv(char *Buffer, size_t Length, int Flag)
{
    if (!m_IsValid || !m_IsConnected)
        return false;

    return recv(m_Socket, Buffer, Length, Flag);
}

ssize_t CTCPSocket::Send(const char *Buffer, size_t Length, int Flag)
{
    if (!m_IsValid || !m_IsConnected)
        return false;

    return send(m_Socket, Buffer, Length, Flag);
}

bool CTCPSocket::Connect(std::string IP, ushort Port)
{
    if (!m_IsValid || m_IsConnected)
        return false;

    sockaddr_in Addr = {};

    Addr.sin_family = AF_INET;
    Addr.sin_port = htons(Port);

    inet_pton(AF_INET, IP.c_str() , &Addr.sin_addr);

    int res = connect(m_Socket, (sockaddr *)&Addr, sizeof(Addr));

    m_IsConnected = res != -1;

    return m_IsConnected;
}

int CTCPSocket::Accpet()
{
    if (!m_IsValid)
        return false;

    sockaddr_in Addr = {};
    socklen_t AddrLen = 0;

    int fd = accept(m_Socket, (sockaddr *)&Addr, &AddrLen);

    return fd;
}

int CTCPSocket::GetSocket()
{
    return m_IsValid ? m_Socket : -1;
}
