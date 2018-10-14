//
// Created by hackerl on 10/12/18.
//

#ifndef SHADOWSOCKSR_CPP_IEVENTLOOP_H
#define SHADOWSOCKSR_CPP_IEVENTLOOP_H

#include "Socket/ISocket.h"

class IEventLoop
{
public:
    virtual bool AddServer(int fd, ISocketAcceptCallback * ServerHandler) = 0;
    virtual bool AddClient(int fd, ISocketEventCallback * ClientHandler) = 0;
    virtual bool Remove(int fd) = 0;
    virtual bool RemoveEvent(int fd) = 0;
    virtual void Loop() = 0;
    virtual bool SetEvent(int fd, short Mode, event_callback_fn OnEvent = nullptr, void * arg = nullptr) = 0;
};
#endif //SHADOWSOCKSR_CPP_IEVENTLOOP_H
