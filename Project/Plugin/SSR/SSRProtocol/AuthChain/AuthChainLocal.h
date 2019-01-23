//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_AUTHCHAINLOCAL_H
#define SSRPLUGIN_AUTHCHAINLOCAL_H

#include "AuthChain.h"
#include <Common/Singleton.h>
#include <Common/Utils/Random.h>

class CAuthChainSession
{
#define g_AuthChainSession SINGLETON_(CAuthChainSession)
public:
    CAuthChainSession()
    {
        m_ClientID = 0;
        m_ConnectionID = 0;
    }

public:
    void LockSession()
    {
        m_Mutex.Lock();
    }

    void UnLockSession()
    {
        m_Mutex.UnLock();
    }

public:
    void SetClientID(u_int ClientID)
    {
        m_ClientID = ClientID;
    }

    void SetConnectionID(u_int ConnectionID)
    {
        m_ConnectionID = ConnectionID;
    }

    u_int GetClientID()
    {
        return m_ClientID;
    }

    u_int GetConnectionID()
    {
        return m_ConnectionID;
    }

private:
    Mutex m_Mutex;
    u_int m_ClientID;
    u_int m_ConnectionID;
};

class CAuthChainLocal : public CAuthChain
{
public:
    CAuthChainLocal()
    {
        m_UserID = g_Random->Next();
    }

public:
    CAuthHeader PackAuthData();
    std::vector<u_char> PackClientData(const u_char *Buffer, size_t Length);

public:
    std::vector<u_char> ClientPack(const u_char *Buffer, size_t Length) override;
    std::vector<u_char> ClientUnPack(const u_char *Buffer, size_t Length) override;

private:
    u_int m_UserID;
};

#endif //SSRPLUGIN_AUTHCHAINLOCAL_H
