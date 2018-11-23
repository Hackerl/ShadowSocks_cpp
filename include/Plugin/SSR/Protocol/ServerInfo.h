//
// Created by patteliu on 2018/11/23.
//

#ifndef SHADOWSOCKSR_CPP_SERVERINFO_H
#define SHADOWSOCKSR_CPP_SERVERINFO_H

#include <string>
#include <vector>

struct CServerInfo
{
    std::string Host;
    u_short Port;
    std::string Param;
    std::vector<u_char> IV;
    std::vector<u_char> RecvIV;
    std::vector<u_char> Key;
    u_int HeadLen;
    u_short TCPMss;
    u_short OverHead;
    u_int BufferSize;

    CServerInfo()
    {
        Port = 0;
        HeadLen = 0;
        TCPMss = 1460;
        OverHead = 0;
        BufferSize = 0;
    }
};

#endif //SHADOWSOCKSR_CPP_SERVERINFO_H
