//
// Created by hackerl on 10/13/18.
//

#include "TCPSocket.h"
#include <Socket/DNS.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>

CTCPSocket::CTCPSocket()
{
    m_Socket = socket(AF_INET, SOCK_STREAM, 0);

    m_IsValid = m_Socket != -1;
    m_IsConnected = false;
}

CTCPSocket::CTCPSocket(int fd, bool IsConnected)
{
    m_Socket = fd;

    m_IsValid = m_Socket != -1;
    m_IsConnected = IsConnected;
}

CTCPSocket::~CTCPSocket()
{
    Close();
}

bool CTCPSocket::Bind(std::string IP, u_short Port)
{
    if (!m_IsValid)
        return false;

    sockaddr_in Address = {};

    Address.sin_family = AF_INET;
    Address.sin_port = htons(Port);

    inet_pton(AF_INET, IP.c_str() , &Address.sin_addr);

    int res = bind(m_Socket, (sockaddr *)&Address, sizeof(Address));

    return res != -1;
}

bool CTCPSocket::Listen(int backlog)
{
    if (!m_IsValid)
        return false;

    int res  = listen(m_Socket, backlog);

    return res != -1;
}

ssize_t CTCPSocket::Recv(void *Buffer, size_t Length, int Flag)
{
    if (!m_IsValid || !m_IsConnected)
        return -1;

    return recv(m_Socket, Buffer, Length, Flag);
}

ssize_t CTCPSocket::Send(const void *Buffer, size_t Length, int Flag)
{
    if (!m_IsValid || !m_IsConnected)
        return -1;

    return send(m_Socket, Buffer, Length, Flag);
}

bool CTCPSocket::Connect(std::string IP, u_short Port, time_t TimeOut)
{
    if (!m_IsValid || m_IsConnected)
        return false;

    std::vector<in_addr> IPList = CDNS::Query(IP.c_str());

    if (IPList.empty())
        return false;

    return Connect(IPList.front().s_addr, htons(Port), TimeOut);
}

bool CTCPSocket::Connect(in_addr_t IP, in_port_t Port, time_t TimeOut)
{
    if (!m_IsValid || m_IsConnected)
        return false;

    sockaddr_in Address = {};

    Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = IP;
    Address.sin_port = Port;

    long ControlFlag = fcntl(m_Socket, F_GETFL, NULL);

    fcntl(m_Socket, F_SETFL, ControlFlag | O_NONBLOCK);

    int res = connect(m_Socket, (sockaddr *)&Address, sizeof(Address));

    if (res < 0 && errno == EINPROGRESS)
    {
        timeval TimeOutVal = {};

        TimeOutVal.tv_sec = TimeOut;
        TimeOutVal.tv_usec = 0;

        fd_set fdSet;

        FD_ZERO(&fdSet);
        FD_SET(m_Socket, &fdSet);

        if (select(m_Socket + 1, nullptr, &fdSet, nullptr, &TimeOutVal) > 0)
        {
            int OptVal = -1;
            socklen_t OptLen = sizeof(OptVal);

            GetSockOpt(SOL_SOCKET, SO_ERROR, &OptVal, &OptLen);

            res = OptVal;
        }
    }

    fcntl(m_Socket, F_SETFL, ControlFlag);

    m_IsConnected = res == 0;

    return m_IsConnected;
}

ITCPSocket * CTCPSocket::Accept()
{
    if (!m_IsValid)
        return nullptr;

    sockaddr_in Address = {};
    socklen_t AddressLen = 0;

    int fd = accept(m_Socket, (sockaddr *)&Address, &AddressLen);

    return new CTCPSocket(fd);
}

int CTCPSocket::GetSocket()
{
    return m_IsValid ? m_Socket : -1;
}

int CTCPSocket::GetSockOpt(int Level, int OptName, void *OptVal, socklen_t *OptLen)
{
    if (!m_IsValid)
        return -1;

    int res = getsockopt(m_Socket, Level, OptName, OptVal, OptLen);

    return res;
}

int CTCPSocket::SetSockOpt(int Level, int OptName, const void * OptVal, socklen_t OptLen)
{
    if (!m_IsValid)
        return -1;

    int res = setsockopt(m_Socket, Level, OptName, OptVal, OptLen);

    return res;
}

bool CTCPSocket::Close()
{
    if (!m_IsValid)
        return false;

    close(m_Socket);

    m_Socket = -1;
    m_IsValid = false;

    return true;
}
