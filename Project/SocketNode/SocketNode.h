//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETNODE_H
#define SHADOWSOCKSR_CPP_SOCKETNODE_H

#include "Node/Node.h"
#include <SocketNode/ISocketNode.h>
#include <Node/INodeManager.h>
#include <Socket/ISocket.h>
#include <EventLoop/IEventLoop.h>
#include <vector>

class CSocketNode : public CNode , public ISocketCallback, public ISocketNode, public INodeEvent
{
public:
    CSocketNode();
    ~CSocketNode();

public:
    void SocketNodeInit(IEventLoop *Loop, IIOSocket *Socket) override;

public:
    virtual bool DataIn(const void *Buffer, size_t Length) = 0;
    virtual bool DataOut(const void *Buffer, size_t Length);

public:
    bool NodeInit(INodeManager * NodeManager) override;
    void OnNodeEvent(NodeEventRegister EventID, void *Context) override;
    void NodeClose() override;


public:
    void OnRead(int fd, short Event) override ;
    void OnWrite(int fd, short Event) override;
    void OnClose(int fd, short Event) override;

protected:
    bool m_Closed;
    short m_Mode;
    IIOSocket * m_Socket;
    std::vector<u_char> m_WriteBuffer;
    IEventLoop * m_Loop;
};

#endif //SHADOWSOCKSR_CPP_SOCKETNODE_H
