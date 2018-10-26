//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETNODE_H
#define SHADOWSOCKSR_CPP_SOCKETNODE_H

#include "Node/Node.h"
#include "SocketNode/ISocketNode.h"
#include <Socket/ISocket.h>
#include <EventLoop/IEventLoop.h>
#include <vector>

class CSocketNode : public CNode , public ISocketClientCallback, public ISocketNode
{
public:
    CSocketNode();
    ~CSocketNode();

public:
    void Init(IEventLoop * Loop, IIOSocket * Socket) override;
    void NodeClose() override;

public:
    virtual bool DataIn(const void *Buffer, size_t Length) = 0;
    virtual bool DataOut(const void *Buffer, size_t Length);

public:
    void OnRead(int fd, short Event) override ;
    void OnWrite(int fd, short Event) override;
    void OnClose(int fd, short Event) override;

protected:
    IIOSocket * m_Socket;
    std::vector<u_char> m_WriteBuffer;
    IEventLoop * m_Loop;
};

#endif //SHADOWSOCKSR_CPP_SOCKETNODE_H
