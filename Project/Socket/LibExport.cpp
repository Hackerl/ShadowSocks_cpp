//
// Created by hackerl on 10/13/18.
//

#include "TCPSocket.h"
#include <Socket/LibSocketExport.h>

extern "C" ITCPSocket * NewTCPSocket()
{
    return new CTCPSocket;
}