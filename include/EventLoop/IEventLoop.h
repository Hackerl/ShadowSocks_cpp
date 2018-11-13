//
// Created by hackerl on 10/12/18.
//

#ifndef SHADOWSOCKSR_CPP_IEVENTLOOP_H
#define SHADOWSOCKSR_CPP_IEVENTLOOP_H

#include "Common/Interface.h"
#include "Socket/ISocket.h"

class IEventLoop : public Interface
{
public:
    virtual bool Add(int fd, ISocketCallback * SocketHandler) = 0;
    virtual bool Remove(int fd) = 0;
    virtual void Loop() = 0;
    virtual bool SetEvent(int fd, short Mode, time_t TimeOut = 0) = 0;
};
#endif //SHADOWSOCKSR_CPP_IEVENTLOOP_H
