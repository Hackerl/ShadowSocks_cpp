//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_SOCKS5_H
#define SHADOWSOCKSR_CPP_SOCKS5_H

#include "Plugin/Plugin.h"

enum CSock5Status
{
    MethodRequestStage = 0,
    ConnectRequestStage,
    ConnectSuccessStage
};

class CSocks5Proxy : public CPlugin
{
public:
    CSocks5Proxy();

public:
    bool SetConfig(Json::Value &Config) override;

public:
    bool OnDataIn(const void *Buffer, size_t Length) override;

public:
    bool MethodRequestHandler(const void *Buffer, size_t Length);
    bool ConnectRequestHandler(const void *Buffer, size_t Length);

private:
    CSock5Status m_Status;
};


#endif //SHADOWSOCKSR_CPP_SOCKS5_H
