//
// Created by patteliu on 2018/11/20.
//

#ifndef SHADOWSOCKSR_CPP_SHADOWSOCKSR_H
#define SHADOWSOCKSR_CPP_SHADOWSOCKSR_H

#include <Socket/ISocket.h>
#include <EventLoop/IEventLoop.h>
#include <Plugin/PluginLoader.h>

struct CShadowConfig
{
    std::string IP;
    u_short Port;
};

class CShadowSocks : public ISocketCallback
{
public:
    CShadowSocks();
    ~CShadowSocks();

public:
    bool SetConfig(Json::Value & Config);

public:
    void OnRead(int fd, short Event) override;
    void OnWrite(int fd ,short Event) override;
    void OnClose(int fd ,short Event) override;

public:
    void Run();

private:
    ITCPSocket * m_Socket;
    IEventLoop * m_EventLoop;
    CShadowConfig m_Config;
    std::vector<IPlugin *> m_ServiceList;
    std::vector<std::string> m_NodeNameList;
};

#endif //SHADOWSOCKSR_CPP_SHADOWSOCKSR_H
