//
// Created by hackerl on 10/29/18.
//

#include "HTTPTunnel.h"
#include "Socket/LibSocketExport.h"

int main()
{
    ITCPSocket * Socket = NewTCPSocket();

    CHTTPTunnel HTTPTunnel;

    HTTPTunnel.SetSocket(Socket);
    HTTPTunnel.SetProxy("127.0.0.1", 8118);

    HTTPTunnel.Connect("www.baidu.com", 443);

    return 0;
}

