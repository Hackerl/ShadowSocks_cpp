//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_PORTTUNNEL_H
#define SHADOWSOCKSR_CPP_PORTTUNNEL_H

#include "Plugin/Plugin.h"
#include "Common/JSONHelper.h"
#include "Socket/LibSocketExport.h"

struct CTunnelConfig
{
    std::string TargetIP;
    u_short TargetPort;
};

class CPortTunnel : public CPlugin
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

    bool OnPipeIn(const void *Buffer, size_t Length) override
    {
        if (!m_IsValid)
        {
            Destroy();
            return false;
        }

        if (!m_HasInit)
        {
            m_HasInit = true;

            ITCPSocket * Socket = NewTCPSocket();

            if (!Socket->Connect(m_Config.TargetIP, m_Config.TargetPort))
            {
                Socket->Close();

                delete Socket;

                Destroy();

                return false;
            }

            NodeInit(Socket);
        }

        return CPlugin::OnPipeIn(Buffer, Length);
    }


private:
    bool m_IsValid;
    bool m_HasInit;
    CTunnelConfig m_Config;
};


#endif //SHADOWSOCKSR_CPP_PORTTUNNEL_H
