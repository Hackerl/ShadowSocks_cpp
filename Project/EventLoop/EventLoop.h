//
// Created by hackerl on 10/12/18.
//

#ifndef SHADOWSOCKSR_CPP_EVENTLOOP_H
#define SHADOWSOCKSR_CPP_EVENTLOOP_H

#include "EventLoop/IEventLoop.h"
#include "Socket/ISocket.h"
#include <map>
#include "Common/Mutex.h"

struct EventSession
{
    event * Event;
    ISocketCallback * SocketHandler;
};

class CEventLoop : public IEventLoop
{
public:
    CEventLoop();
    ~CEventLoop();

public:
    bool Add(int fd, ISocketCallback * SocketHandler) override;
    bool Remove(int fd) override;
    bool SetEvent(int fd, short Mode, time_t TimeOut) override;
    void Loop() override;
    void Destroy();

private:
    Mutex m_Mutex;
    event_base * m_EventBase;
    std::map<int, EventSession> m_SocketEventMap;
};

#endif //SHADOWSOCKSR_CPP_EVENTLOOP_H
