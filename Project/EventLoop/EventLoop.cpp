//
// Created by hackerl on 10/12/18.
//

#include "EventLoop.h"

CEventLoop::CEventLoop() {
    m_EventBase = event_base_new();
}

bool CEventLoop::AddServer(int fd, ISocketAcceptCallback * ServerHandler) {

    struct Stub
    {
        static void OnAceept(int fd ,short Event, void* arg)
        {
            auto ServerHandler = (ISocketAcceptCallback*) arg;
            ServerHandler->OnAccpet(fd, Event);
        }
    };

    event* Event = event_new(m_EventBase, fd , EV_READ | EV_PERSIST, Stub::OnAceept , ServerHandler);

    event_add(Event, nullptr);

    return true;
}

bool CEventLoop::AddClient(int fd, ISocketEventCallback *ClientHandler) {

    struct Stub
    {
        static void OnRead(bufferevent* BufEvent,void* arg)
        {
            auto ClientHandler = (ISocketEventCallback*) arg;
            ClientHandler->OnRead(BufEvent);
        }

        static void OnWrite(bufferevent* BufEvent,void* arg)
        {
            auto ClientHandler = (ISocketEventCallback*) arg;
            ClientHandler->OnWrite(BufEvent);
        }

        static void OnError(bufferevent* BufEvent, short event, void* arg)
        {
            auto ClientHandler = (ISocketEventCallback*) arg;
            ClientHandler->OnError(BufEvent, event);
        }
    };

    bufferevent* buffer_ev = bufferevent_socket_new(m_EventBase, fd, BEV_OPT_CLOSE_ON_FREE);

    bufferevent_setcb(buffer_ev, Stub::OnRead , Stub::OnWrite, Stub::OnError, ClientHandler);

    bufferevent_enable(buffer_ev,EV_READ | EV_PERSIST);

    return false;
}

bool CEventLoop::Remove(int fd) {
    return false;
}

void CEventLoop::Loop() {
    event_base_dispatch(m_EventBase);
}
