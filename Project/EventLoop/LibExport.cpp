//
// Created by hackerl on 10/13/18.
//

#include "EventLoop.h"
#include <EventLoop/LibEventExport.h>

extern "C" IEventLoop * NewEventLoop()
{
    return new CEventLoop;
}

extern "C" void DeleteEventLoop(IEventLoop * EventLoop)
{
    auto Loop = dynamic_cast<CEventLoop *>(EventLoop);
    delete Loop;
}