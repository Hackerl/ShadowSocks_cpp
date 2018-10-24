//
// Created by patteliu on 2018/10/24.
//

#include "Socket/ISocket.h"
#include "Socket/LibSocketExport.h"
#include "EventLoop/LibEventExport.h"
#include "SocketRelay/LibSocketRelayExport.h"
#include "Plugin/LibPluginExport.h"
#include "Common/JSONHelper.h"

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
        ITCPSocket * Local = m_Socket->Accept();
        std::cout << "Accept" << std::endl;

        IPlugin * Socks5Proxy = NewSocks5Proxy();

        ISocketRelay * TCPRelay1 = NewSocketRelay();
        TCPRelay1->Init(m_Loop, Local);
        TCPRelay1->SetPlugin(Socks5Proxy);

        Json::Value Config;

        Config["TargetIP"] = "127.0.0.1";
        Config["TargetPort"] = 4444;

        IPlugin * PortTunnel = NewPortTunnel();

        PortTunnel->SetConfig(Config);

        ISocketRelay * TCPRelay2 = NewSocketRelay();
        TCPRelay2->Init(m_Loop, nullptr);
        TCPRelay2->SetPlugin(PortTunnel);

        PairPipeConnect(TCPRelay1, TCPRelay2);
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