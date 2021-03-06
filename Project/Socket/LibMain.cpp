//
// Created by hackerl on 10/13/18.
//

#include "TCPSocket.h"
#include <Socket/ISocket.h>
#include <Socket/LibSocketExport.h>
#include <EventLoop/LibEventExport.h>
#include <iostream>

class CServer : public CTCPSocket , public ISocketCallback
{
    void OnRead(int fd, short Event) override
    {
        Accept();
        std::cout << "Accept" << std::endl;
    }

    void OnWrite(int fd ,short Event) override
    {

    }

    void OnClose(int fd ,short Event) override
    {

    }
};

void TestServer()
{
    CServer Server;

    if (!Server.Bind("0.0.0.0", 3333))
        std::cout << "Bind Faild" << std::endl;

    if (!Server.Listen(255))
        std::cout << "Listen Faild" << std::endl;

    IEventLoop * Loop = NewEventLoop();

    Loop->Add(Server.GetSocket(), &Server);

    Loop->Loop();

    DeleteEventLoop(Loop);
}

void TestClient()
{
    ITCPSocket * Client = NewTCPSocket();

    do
    {
        if (!Client->Connect("127.0.0.1", 3333))
        {
            std::cout << "Connect Faild" << std::endl;
            break;
        }

        std::string data= "Hello";

        Client->Send(data.data(), data.length(), 0);

    } while (false);

    delete Client;
}

class CClient : public CTCPSocket , public ISocketCallback
{
public:
    CClient()
    {
        m_Loop = nullptr;
    }

public:
    void OnRead(int fd ,short Event) override
    {
        std::cout << "Can Read" << std::endl;

        char Buffer[1024];

        int len = Recv(Buffer, 1024, 0);

        m_Loop->SetEvent(fd, EV_READ| EV_WRITE | EV_PERSIST);
    }

    void OnWrite(int fd ,short Event) override
    {
        std::cout << "Can Write" << std::endl;
    }

    void OnClose(int fd ,short Event) override
    {
        std::cout << "Closed" << std::endl;
        m_Loop->Remove(fd);
    }

public:
    IEventLoop * m_Loop;
};

void TestEventCallback()
{
    CClient Client;

    do
    {
        if (!Client.Connect("127.0.0.1", 3333, 2))
        {
            std::cout << "Connect Faild" << std::endl;
            break;
        }

        std::string data= "Hello";

        Client.Send(data.data(), data.length(), 0);

        IEventLoop * Loop = NewEventLoop();

        Client.m_Loop = Loop;

        Loop->Add(Client.GetSocket(), &Client);

        //Loop->AddServer(Server.GetSocket(), &Server);

        Loop->Loop();

        DeleteEventLoop(Loop);

    } while (false);
}

int main()
{
    TestEventCallback();
    return 0;
}