//
// Created by patteliu on 2018/10/24.
//

#include "Node/INode.h"
#include "Socket/ISocket.h"
#include "Socket/LibSocketExport.h"
#include "EventLoop/LibEventExport.h"
#include "Plugin/LibPluginExport.h"
#include "Node/NodeManager.h"
#include "SocketNode/LibSocketNodeExport.h"

class CServer : public ISocketCallback
{
public:
    CServer(ITCPSocket * Socket, IEventLoop * Loop)
    {
        m_Socket = Socket;
        m_Loop = Loop;
    }

    ~CServer()
    {
        delete m_Socket;
    }

public:
    void OnRead(int fd, short Event) override
    {

    }

    void OnWrite(int fd ,short Event) override
    {

    }

    void OnClose(int fd ,short Event) override
    {
        m_Loop->Remove(m_Socket->GetSocket());
    }

private:
    ITCPSocket * m_Socket;
    IEventLoop * m_Loop;
};

void TestServer()
{
    ITCPSocket * Socket = NewTCPSocket();

    if (!Socket->Bind("0.0.0.0", 1080))
    {
        std::cout << "Bind Faild" << std::endl;
        return;
    }

    if (!Socket->Listen(255))
    {
        std::cout << "Listen Faild" << std::endl;
        return;
    }

    IEventLoop * Loop = NewEventLoop();

    CServer Server(Socket, Loop);

    Loop->Add(Socket->GetSocket(), &Server);

    Loop->Loop();

    delete Loop;
}

int main()
{
    TestServer();
    return 0;
}