//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_PROTOCOL_H
#define SSRPLUGIN_PROTOCOL_H

#include <iostream>
#include <vector>

struct CServerInfo
{
    std::string Host;
    ushort Port;
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
        TCPMss = 1440;
        OverHead = 0;
        BufferSize = 0;
    }
};

class CProtocol
{
public:
    CProtocol()
    {
        m_OverHead = 0;
    }

public:
    void SetServerInfo(CServerInfo & Info)
    {
        m_ServerInfo = Info;
    }

    u_short GetOverHead()
    {
        return m_OverHead;
    }

protected:
    CServerInfo m_ServerInfo;
    u_short m_OverHead;
};

#endif //SSRPLUGIN_PROTOCOL_H
