//
// Created by patteliu on 2018/10/24.
//

#include "Node/INode.h"
#include "Socket/ISocket.h"
#include "Socket/LibSocketExport.h"
#include "EventLoop/LibEventExport.h"
#include "Plugin/LibPluginExport.h"
#include "SocketNode/LibSocketNodeExport.h"

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

        ISocketNode * LocalNode = NewLocalSocketNode();
        LocalNode->Init(m_Loop, Local);

        IPlugin * Socks5Proxy = NewSocks5Proxy();
        IPlugin * ProxyServer = NewProxyServer();

        ISocketNode * RemoteNode = NewRemoteSocketNode();
        RemoteNode->Init(m_Loop);

        CNodeManager NodeMgr;

        NodeMgr.AddNode(LocalNode);
        NodeMgr.AddNode(Socks5Proxy);
        NodeMgr.AddNode(ProxyServer);
        NodeMgr.AddNode(RemoteNode);

        NodeMgr.Connect();
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