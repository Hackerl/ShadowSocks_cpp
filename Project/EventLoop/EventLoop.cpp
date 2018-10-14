//
// Created by hackerl on 10/12/18.
//

#include "EventLoop.h"

CEventLoop::CEventLoop()
{
    m_EventBase = event_base_new();
}

CEventLoop::~CEventLoop()
{
    event_base_free(m_EventBase);
    m_EventBase = nullptr;
}

bool CEventLoop::AddServer(int fd, ISocketAcceptCallback * ServerHandler)
{
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

    m_SocketEventMap.insert(std::pair<int, event*>(fd, Event));

    return true;
}

bool CEventLoop::AddClient(int fd, ISocketEventCallback * ClientHandler)
{
    struct Stub
    {

        static void OnEvent(int fd ,short Event, void* arg)
        {
            auto ClientHandler = (ISocketEventCallback *) arg;

            do
            {
                if (Event & EV_READ)
                    ClientHandler->OnRead(fd, Event);

                if (Event & EV_WRITE)
                    ClientHandler->OnWrite(fd, Event);

                if (Event & EV_CLOSED)
                    ClientHandler->OnClose(fd, Event);

            } while (false);

        }
    };

    event* Event = event_new(m_EventBase, fd, EV_READ | EV_CLOSED |EV_PERSIST, Stub::OnEvent , ClientHandler);

    event_add(Event, nullptr);

    m_SocketEventMap.insert(std::pair<int, event*>(fd, Event));

    return false;
}

bool CEventLoop::SetEvent(int fd, short Mode, event_callback_fn OnEvent, void * arg)
{
    auto Iterator = m_SocketEventMap.find(fd);

    if(Iterator == m_SocketEventMap.end())
        return false;

    event * Event = Iterator->second;

    event_set(
            Event,
            fd,
            Mode,
            OnEvent == nullptr ? event_get_callback(Event) : OnEvent,
            arg == nullptr ? event_get_callback_arg(Event) : arg
            );
}

bool CEventLoop::Remove(int fd)
{
    auto Iterator = m_SocketEventMap.find(fd);

    if(Iterator == m_SocketEventMap.end())
        return false;

    event * Event = Iterator->second;

    event_del(Event);
    event_free(Event);

    m_SocketEventMap.erase(fd);

    return true;
}

void CEventLoop::Loop()
{
    event_base_dispatch(m_EventBase);
}

void CEventLoop::Destroy()
{
    for (auto const& Iterator : m_SocketEventMap)
    {
        event_del(Iterator.second);
        event_free(Iterator.second);
    }

    m_SocketEventMap.clear();

    event_base_loopbreak(m_EventBase);
}