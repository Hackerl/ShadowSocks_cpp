//
// Created by hackerl on 10/29/18.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKETPROXY_H
#define SHADOWSOCKSR_CPP_ISOCKETPROXY_H

#include <iostream>
#include "Socket/ISocket.h"
#include "Common/Interface.h"

class ISocketProxy : public Interface
{
public:
    virtual void SetSocket(ITCPSocket * Socket) = 0;
    virtual void SetProxy(std::string IP, u_short Port) = 0;
    virtual bool Connect(std::string IP, ushort Port) = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKETPROXY_H
