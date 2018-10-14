//
// Created by hackerl on 10/12/18.
//

#ifndef SHADOWSOCKSR_CPP_EVENTLOOP_H
#define SHADOWSOCKSR_CPP_EVENTLOOP_H

#include "EventLoop/IEventLoop.h"
#include "Socket/ISocket.h"
#include <map>

class CEventLoop : public IEventLoop
{
public:
    CEventLoop();
    ~CEventLoop();

public:
    bool AddServer (int fd, ISocketAcceptCallback * ServerHandler) override;
    bool AddClient (int fd, ISocketEventCallback * ClientHandler) override;
    bool Remove(int fd) override;
    bool RemoveEvent(int fd) override;
    bool SetEvent(int fd, short Mode, event_callback_fn OnEvent, void * arg) override;
    void Loop() override;
    void Destroy();

private:
    event_base * m_EventBase;
    std::map<int, event*> m_SocketEventMap;
};

#endif //SHADOWSOCKSR_CPP_EVENTLOOP_H
