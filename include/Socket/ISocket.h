//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKET_H
#define SHADOWSOCKSR_CPP_ISOCKET_H

#include <event.h>
#include <iostream>

class IIOSocket
{
public:
    virtual ssize_t Recv(char * Buffer, size_t Length, int Flag = 0) = 0;
    virtual ssize_t Send(const char * Buffer, size_t Length, int Flag = 0) = 0;
    virtual bool Close() = 0;
};

class ITCPSocket : public IIOSocket
{
public:
    virtual bool Bind(std::string IP, ushort Port) = 0;
    virtual bool Listen(int backlog = 255) = 0;
    virtual bool Connect(std::string IP, ushort Port) = 0;
    virtual int Accpet() = 0;

};

class IUDPSocket : public IIOSocket
{
public:
    virtual bool Bind(std::string IP, ushort Port) = 0;
};

class ISocketEventCallback
{
public:
    virtual void OnRead(int fd ,short Event) = 0;
    virtual void OnWrite(int fd ,short Event) = 0;
    virtual void OnClose(int fd ,short Event) = 0;
};

class ISocketAcceptCallback
{
public:
    virtual void OnAccpet(int fd ,short Event) = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKET_H
