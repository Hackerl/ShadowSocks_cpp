//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_LIBSOCKETEXPORT_H
#define SHADOWSOCKSR_CPP_LIBSOCKETEXPORT_H

#include "ISocket.h"

extern "C" ITCPSocket * NewTCPSocket();
extern "C" void DeleteTCPSocket(ITCPSocket * EventLoop);

#endif //SHADOWSOCKSR_CPP_LIBSOCKETEXPORT_H
