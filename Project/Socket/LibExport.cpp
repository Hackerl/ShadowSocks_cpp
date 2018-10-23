//
// Created by hackerl on 10/13/18.
//

#include "Socket/LibSocketExport.h"
#include "TCPSocket.h"

extern "C" ITCPSocket * NewTCPSocket()
{
    return new CTCPSocket;
}

extern "C" ITCPSocket * GetTCPSocket(int fd, bool IsConnected)
{
    return new CTCPSocket(fd, IsConnected);
}

extern "C" void DeleteTCPSocket(ITCPSocket * EventLoop)
{
    auto * Socket = dynamic_cast<CTCPSocket *>(EventLoop);
    delete Socket;
}