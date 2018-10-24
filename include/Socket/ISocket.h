//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKET_H
#define SHADOWSOCKSR_CPP_ISOCKET_H

#include <event.h>
#include <iostream>
#include "Common/Interface.h"

class IIOSocket : public Interface
{
public:
    virtual int GetSocket() = 0;
    virtual ssize_t Recv(void *Buffer, size_t Length, int Flag = 0) = 0;
    virtual ssize_t Send(const void *Buffer, size_t Length, int Flag = 0) = 0;
    virtual bool Close() = 0;
};

class ITCPSocket : public IIOSocket
{
public:
    virtual bool Bind(std::string IP, ushort Port) = 0;
    virtual bool Listen(int backlog = 255) = 0;
    virtual bool Connect(std::string IP, ushort Port) = 0;
    virtual ITCPSocket * Accept() = 0;
};

class IUDPSocket : public IIOSocket
{
public:
    virtual bool Bind(std::string IP, ushort Port) = 0;
};

class ISocketCloseCallback : public Interface
{
public:
    virtual void OnClose(int fd ,short Event) = 0;
};

class ISocketClientCallback : public ISocketCloseCallback
{
public:
    virtual void OnRead(int fd ,short Event) = 0;
    virtual void OnWrite(int fd ,short Event) = 0;
    void OnClose(int fd ,short Event) override = 0;
};

class ISocketServerCallback : public ISocketCloseCallback
{
public:
    virtual void OnAccept(int fd, short Event) = 0;
    void OnClose(int fd ,short Event) override = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKET_H
