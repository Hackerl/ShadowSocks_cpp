//
// Created by hackerl on 10/12/18.
//

#ifndef SHADOWSOCKSR_CPP_EVENTLOOP_H
#define SHADOWSOCKSR_CPP_EVENTLOOP_H

#include "EventLoop/IEventLoop.h"

class CEventLoop : public IEventLoop
{
public:
    CEventLoop();
    ~CEventLoop() = default;

public:
    bool AddServer (int fd, ISocketAcceptCallback * ServerHandler) override;
    bool AddClient (int fd, ISocketEventCallback * ClientHandler) override;
    bool Remove(int fd) override;
    void Loop() override;

private:
    event_base * m_EventBase;
};

#endif //SHADOWSOCKSR_CPP_EVENTLOOP_H
