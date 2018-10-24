//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_COMMONSOCKETRELAY_H
#define SHADOWSOCKSR_CPP_COMMONSOCKETRELAY_H

#include "Socket/ISocket.h"
#include "EventLoop/IEventLoop.h"
#include "Pipe/Pipe.h"
#include "Plugin/IPlugin.h"
#include <vector>
#include <SocketRelay/ISocketRelay.h>

class CCommonSocketRelay :public ISocketRelay
{
public:
    CCommonSocketRelay();
    ~CCommonSocketRelay();

public:
    void Init(IIOSocket * Socket, IEventLoop * Loop);
    void Init(IEventLoop * Loop);
    void PipeInit(void * args) override;

public:
    bool PipeOut(const void *Buffer, size_t Length) override;
    void OnPipeClose() override;

public:
    void OnRead(int fd, short Event) override ;
    void OnWrite(int fd, short Event) override;
    void OnClose(int fd, short Event) override;

private:
    IIOSocket * m_Socket;
    std::vector<u_char> m_WriteBuffer;
    IEventLoop * m_Loop;
};

#endif //SHADOWSOCKSR_CPP_COMMONSOCKETRELAY_H
