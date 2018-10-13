//
// Created by hackerl on 10/13/18.
//

#ifndef SHADOWSOCKSR_CPP_LIBEVENTEXPORT_H
#define SHADOWSOCKSR_CPP_LIBEVENTEXPORT_H

#include "IEventLoop.h"

extern "C" IEventLoop * NewEventLoop();
extern "C" void DeleteEventLoop(IEventLoop * EventLoop);

#endif //SHADOWSOCKSR_CPP_LIBEVENTEXPORT_H
