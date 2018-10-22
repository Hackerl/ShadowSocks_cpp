//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_TCPRELAY_H
#define SHADOWSOCKSR_CPP_TCPRELAY_H

#include "Socket/ISocket.h"
#include "EventLoop/IEventLoop.h"
#include "Pipe/Pipe.h"
#include <vector>

class CTCPRelay :public CPipe, public ISocketClientCallback
{
public:
    CTCPRelay();
    ~CTCPRelay();

public:
    void Init(IIOSocket * Socket, IEventLoop * Loop);
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


#endif //SHADOWSOCKSR_CPP_TCPRELAY_H
