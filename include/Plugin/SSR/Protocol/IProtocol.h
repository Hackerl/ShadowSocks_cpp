//
// Created by patteliu on 2018/11/23.
//

#ifndef SHADOWSOCKSR_CPP_IPROTOCOL_H
#define SHADOWSOCKSR_CPP_IPROTOCOL_H

#include "ServerInfo.h"
#include <Common/Interface.h>

class IProtocol : public Interface
{
public:
    virtual u_short GetOverHead() = 0;
    virtual void SetServerInfo(CServerInfo & Info) = 0;

public:
    virtual bool NeedSendBack() = 0;
    virtual std::vector<u_char> PackSendBackData() = 0;

public:
    virtual std::vector<u_char> ClientPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ClientUnPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ServerPack(const u_char *Buffer, size_t Length) = 0;
    virtual std::vector<u_char> ServerUnPack(const u_char *Buffer, size_t Length) = 0;
};

#endif //SHADOWSOCKSR_CPP_IPROTOCOL_H
