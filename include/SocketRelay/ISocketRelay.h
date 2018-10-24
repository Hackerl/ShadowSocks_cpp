//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKETRELAY_H
#define SHADOWSOCKSR_CPP_ISOCKETRELAY_H

#include "Common/Interface.h"
#include "Plugin/IPlugin.h"
#include "Socket/ISocket.h"
#include "EventLoop/IEventLoop.h"

class ISocketRelay : public Interface
{
public:
    virtual void Init(IEventLoop * Loop, IIOSocket * Socket = nullptr) = 0;
    virtual void SetPlugin(IPlugin * Plugin) = 0;
};


#endif //SHADOWSOCKSR_CPP_ISOCKETRELAY_H
