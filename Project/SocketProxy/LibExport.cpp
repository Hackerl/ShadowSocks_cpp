//
// Created by hackerl on 10/29/18.
//

#include "HTTPTunnel.h"
#include "SocketProxy/LibSocketProxyExport.h"

extern "C" ISocketProxy * NewHTTPTunnel()
{
    return new CHTTPTunnel;
}