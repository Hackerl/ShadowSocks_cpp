//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_PORTTUNNEL_H
#define SHADOWSOCKSR_CPP_PORTTUNNEL_H

#include <Plugin/IPlugin.h>
#include <Node/Node.h>
#include <Node/NodeServiceDef.h>
#include <Common/JSONHelper.h>
#include <Socket/LibSocketExport.h>

struct CTunnelConfig
{
    std::string TargetIP;
    u_short TargetPort;
};

class CPortTunnel : public IPlugin, public CNode
{
public:
    CPortTunnel();

public:
    bool InitPlugin(const void *Context, size_t Size) override;

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;

private:
    bool m_IsValid;
    bool m_HasInit;
    CTunnelConfig m_Config;
};


#endif //SHADOWSOCKSR_CPP_PORTTUNNEL_H
