//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKET_H
#define SHADOWSOCKSR_CPP_ISOCKET_H

#include <event.h>

class ISocket
{
public:
    virtual bool Bind() = 0;
    virtual bool Listen() = 0;
    virtual bool Rev() = 0;
    virtual bool Send() = 0;
};

class ISocketEventCallback
{
public:
    virtual void OnRead(bufferevent* BufEvent) = 0;
    virtual void OnWrite(bufferevent* BufEvent) = 0;
    virtual void OnError(bufferevent* BufEvent, short Event) = 0;
};

class ISocketAcceptCallback
{
public:
    virtual void OnAccpet(int fd ,short Event) = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKET_H
