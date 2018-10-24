//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_COMMONSOCKETRELAY_H
#define SHADOWSOCKSR_CPP_COMMONSOCKETRELAY_H

#include "Pipe/Pipe.h"
#include "SocketRelay/ISocketRelay.h"
#include <vector>

class CCommonSocketRelay : public CPipe, public ISocketClientCallback, public ISocketRelay
{
public:
    CCommonSocketRelay();
    ~CCommonSocketRelay();

public:
    void Init(IEventLoop * Loop, IIOSocket * Socket);
    void SetPlugin(IPlugin * Plugin) override;

public:
    void PipeInit(void * args) override;
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
