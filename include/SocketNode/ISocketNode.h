//
// Created by patteliu on 2018/10/26.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKETNODE_H
#define SHADOWSOCKSR_CPP_ISOCKETNODE_H

#include <Common/Interface.h>
#include <EventLoop/IEventLoop.h>

class ISocketNode : public Interface
{
public:
    virtual void SocketNodeInit(IEventLoop *Loop, IIOSocket *Socket = nullptr) = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKETNODE_H
