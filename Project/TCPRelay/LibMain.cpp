//
// Created by hackerl on 10/14/18.
//

#include "Socket/ISocket.h"
#include "Socket/LibSocketExport.h"
#include "EventLoop/LibEventExport.h"
#include "TCPRelay.h"
#include "Common/InstanceManager.h"

class CServer : public ISocketServerCallback
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
    void OnAccept(int fd, short Event) override
    {
        int Client = m_Socket->Accept();
        std::cout << "Accept" << std::endl;

        ITCPSocket * Local = NewTCPSocket();
        Local->SetSocket(Client);

        ITCPSocket * Remote = NewTCPSocket();

        if (!Remote->Connect("127.0.0.1", 2222))
        {
            std::cout << "Connect Faild" << std::endl;
            Local->Close();
            Remote->Close();

            delete Local;
            delete Remote;
            return;
        }

        CTCPRelay * TCPRelay1 = new InstanceManager<CTCPRelay>;
        TCPRelay1->Init(Local, m_Loop);

        CTCPRelay * TCPRelay2 = new InstanceManager<CTCPRelay>;
        TCPRelay2->Init(Remote, m_Loop);

        TCPRelay1->PipeConnect(TCPRelay2);
        TCPRelay2->PipeConnect(TCPRelay1);

        m_Loop->AddClient(Local->GetSocket(), TCPRelay1);
        m_Loop->AddClient(Remote->GetSocket(), TCPRelay2);
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

    if (!Socket->Bind("0.0.0.0", 3333))
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

    Loop->AddServer(Socket->GetSocket(), &Server);

    Loop->Loop();

    delete Loop;
}

int main()
{
    TestServer();
    return 0;
}