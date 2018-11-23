//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_PROTOCOL_H
#define SSRPLUGIN_PROTOCOL_H

#include "IProtocol.h"

class CProtocol : public IProtocol
{
public:
    CProtocol()
    {
        m_OverHead = 0;
    }

public:
    void SetServerInfo(CServerInfo & Info) override
    {
        m_ServerInfo = Info;
    }

    u_short GetOverHead() override
    {
        return m_OverHead;
    }

public:
    bool NeedSendBack() override
    {
        return false;
    }

    std::vector<u_char> PackSendBackData() override
    {
        return std::vector<u_char>();
    }

public:
    std::vector<u_char> ClientPack(const u_char *Buffer, size_t Length) override
    {
        return std::vector<u_char>(Buffer, Buffer + Length);
    }

    std::vector<u_char> ClientUnPack(const u_char *Buffer, size_t Length) override
    {
        return std::vector<u_char>(Buffer, Buffer + Length);
    }

    std::vector<u_char> ServerPack(const u_char *Buffer, size_t Length) override
    {
        return std::vector<u_char>(Buffer, Buffer + Length);
    }

    std::vector<u_char> ServerUnPack(const u_char *Buffer, size_t Length) override
    {
        return std::vector<u_char>(Buffer, Buffer + Length);
    }

protected:
    u_short m_OverHead;
    CServerInfo m_ServerInfo;
};

#endif //SSRPLUGIN_PROTOCOL_H
