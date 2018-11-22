//
// Created by hackerl on 11/4/18.
//

#ifndef SSRPLUGIN_TLSTICKETAUTH_H
#define SSRPLUGIN_TLSTICKETAUTH_H

#include <netinet/in.h>
#include <vector>
#include <sys/param.h>
#include <Protocol/Protocol.h>
#include <Utils/Random.h>

class CTLSAuthSession
{
#define g_TLSAuthSession SINGLETON_(CTLSAuthSession)
public:
    CTLSAuthSession()
    {
        for (int i = 0; i < 32; i ++)
            m_ClientID.push_back(u_char(g_Random->Next() % 256));

        auto Count = u_short(g_Random->Next());

        for (int i = 0; i < (Count % 17 + 8) * 16; i ++)
            m_TicketBuf.push_back(u_char(g_Random->Next() % 256));
    }

public:
    std::vector<u_char> GetClientID()
    {
        return m_ClientID;
    }

    std::vector<u_char> GetTicketBuf()
    {
        return m_TicketBuf;
    }

private:
    std::vector<u_char> m_ClientID;
    std::vector<u_char> m_TicketBuf;
};

class CTLSTicketAuth : public CProtocol
{
public:
    CTLSTicketAuth();
    ~CTLSTicketAuth();

public:
    bool NeedSendBack();
    std::vector<u_char> PackAuthData();
    std::vector<u_char> PackSendBackData();
    std::vector<u_char> SNI(std::string Host);

public:
    std::vector<u_char> Encode(u_char * Buffer, size_t Length);
    std::vector<u_char> Decode(u_char * Buffer, size_t Length);

private:
    int m_Status;
    bool m_NeedSendBack;
    std::vector<u_char> m_RecvBuffer;
    std::vector<u_char> m_SendBuffer;
};

#endif //SSRPLUGIN_TLSTICKETAUTH_H
