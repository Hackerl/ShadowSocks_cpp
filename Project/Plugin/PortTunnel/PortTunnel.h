//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_PORTTUNNEL_H
#define SHADOWSOCKSR_CPP_PORTTUNNEL_H

#include "Plugin/IPlugin.h"
#include "Node/Node.h"
#include "Node/NodeService.h"
#include "Common/JSONHelper.h"
#include "Socket/LibSocketExport.h"

struct CTunnelConfig
{
    std::string TargetIP;
    u_short TargetPort;
};

class CPortTunnel : public IPlugin, public CNode
{
public:
    CPortTunnel() : m_Config()
    {
        m_HasInit = false;
        m_IsValid = false;
    }

public:
    bool SetConfig(Json::Value &Config) override
    {
        u_int Port = g_JSON->GetUInt(Config, "TargetPort", 0x10000);
        std::string IP = g_JSON->GetString(Config, "TargetIP");

        if (Port > 0xffff || IP.empty())
            return false;

        m_Config.TargetIP = IP;
        m_Config.TargetPort = (u_short) Port;

        m_IsValid = true;

        return true;
    }

    bool OnUpStream(const void *Buffer, size_t Length) override
    {
        if (!m_IsValid)
            return false;

        if (!m_HasInit)
        {
            m_HasInit = true;

            ITCPSocket * Socket = NewTCPSocket();

            if (!Socket->Connect(m_Config.TargetIP, m_Config.TargetPort))
            {
                Socket->Close();

                delete Socket;

                return false;
            }

            m_NodeManager->InvokeService(INIT_REMOTE_SOCKET, Socket);
        }

        return UpStream(Buffer, Length);
    }

private:
    bool m_IsValid;
    bool m_HasInit;
    CTunnelConfig m_Config;
};


#endif //SHADOWSOCKSR_CPP_PORTTUNNEL_H
