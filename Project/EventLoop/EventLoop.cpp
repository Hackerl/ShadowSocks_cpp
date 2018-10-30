//
// Created by hackerl on 10/12/18.
//

#include "EventLoop.h"
#include "Common/IInstanceManager.h"

CEventLoop::CEventLoop()
{
    m_EventBase = event_base_new();
}

CEventLoop::~CEventLoop()
{
    event_base_free(m_EventBase);
    m_EventBase = nullptr;
}

bool CEventLoop::AddServer(int fd, ISocketServerCallback * ServerHandler)
{
    struct Stub
    {
        static void OnAccept(int fd, short Event, void *arg)
        {
            auto ServerHandler = (ISocketServerCallback*) arg;
            ServerHandler->OnAccept(fd, Event);
        }
    };

    event* Event = event_new(m_EventBase, fd, EV_READ | EV_PERSIST, Stub::OnAccept, ServerHandler);

    event_add(Event, nullptr);

    EventSession Session = {};

    Session.Event = Event;
    Session.CloseHandler = (ISocketCloseCallback *) ServerHandler;

    AutoMutex _0_(m_Mutex);
    m_SocketEventMap.insert(std::pair<int, EventSession>(fd, Session));

    AddRef(ServerHandler);

    return true;
}

bool CEventLoop::AddClient(int fd, ISocketClientCallback * ClientHandler)
{
    struct Stub
    {
        static void OnEvent(int fd ,short Event, void* arg)
        {
            auto ClientHandler = (ISocketClientCallback *) arg;

            if (Event & EV_READ)
            {
                ClientHandler->OnRead(fd, Event);
                return;
            }

            if (Event & EV_WRITE)
            {
                ClientHandler->OnWrite(fd, Event);
                return;
            }

            if (Event & EV_CLOSED)
            {
                ClientHandler->OnClose(fd, Event);
                return;
            }
        }
    };

    event* Event = event_new(m_EventBase, fd, EV_READ | EV_CLOSED | EV_PERSIST, Stub::OnEvent , ClientHandler);

    event_add(Event, nullptr);

    EventSession Session = {};

    Session.Event = Event;
    Session.CloseHandler = (ISocketCloseCallback *) ClientHandler;

    AutoMutex _0_(m_Mutex);

    m_SocketEventMap.insert(std::pair<int, EventSession>(fd, Session));

    AddRef(ClientHandler);

    return true;
}

bool CEventLoop::SetEvent(int fd, short Mode)
{
    AutoMutex _0_(m_Mutex);

    auto Iterator = m_SocketEventMap.find(fd);

    if(Iterator == m_SocketEventMap.end())
        return false;

    EventSession& Session = Iterator->second;

    event_del(Session.Event);

    Session.Event->ev_events = Mode;

    event_add(Session.Event, nullptr);

    return true;
}

bool CEventLoop::Remove(int fd)
{
    AutoMutex _0_(m_Mutex);

    auto Iterator = m_SocketEventMap.find(fd);

    if(Iterator == m_SocketEventMap.end())
        return false;

    EventSession & Session = Iterator->second;

    event_del(Session.Event);
    event_free(Session.Event);

    Release(Session.CloseHandler);

    m_SocketEventMap.erase(Iterator);

    return true;
}

void CEventLoop::Loop()
{
    event_base_dispatch(m_EventBase);
}

void CEventLoop::Destroy()
{
    AutoMutex _0_(m_Mutex);

    for (auto const& Iterator : m_SocketEventMap)
    {
        const EventSession& Session = Iterator.second;

        event_del(Session.Event);
        event_free(Session.Event);

        Session.CloseHandler->OnClose(Iterator.first, EV_CLOSED);

        Release(Session.CloseHandler);
    }

    m_SocketEventMap.clear();

    event_base_loopbreak(m_EventBase);
}