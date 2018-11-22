//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_AUTHCHAIN_H
#define SSRPLUGIN_AUTHCHAIN_H

#include <cstring>
#include <Utils/HMAC.h>
#include <Utils/Encryptor.h>
#include <Utils/RC4.h>
#include <Protocol/Protocol.h>
#include <Utils/XorShift128Plus.h>
#include <Utils/AES128.h>

#pragma pack(push, 1)

struct CClientInfo
{
    u_int UTC;
    u_int ClientID;
    u_int ConnectionID;
    u_short OverHead;
    u_short Reserve;

    CClientInfo()
    {
        UTC = (u_int)time(nullptr);
        ClientID = 0;
        ConnectionID = 0;
        OverHead = 0;
        Reserve = 0;
    }

    void SetInfo(u_int CID, u_int ConID, u_short Over)
    {
        ClientID = CID;
        ConnectionID = ConID;
        OverHead = Over;
    }
};

struct CAuthHeader
{
    u_int CheckHead;
    u_char HeadHMACMd5[8];
    u_int UserID;
    CClientInfo ClientInfo;
    u_char InfoHMACMd5[4];

    CAuthHeader() : HeadHMACMd5(), InfoHMACMd5()
    {
        UserID = 0;
        CheckHead = 0;
    }

    void EncryptInfo(std::vector<u_char> & AESKey)
    {
        CClientInfo NewClientInfo;

        CAES128 _0_;

        _0_.Init(AESKey.data(), AESKey.size());

        _0_.CBCEncrypt((u_char *)&ClientInfo, (u_char *)&NewClientInfo, sizeof(ClientInfo));

        ClientInfo = NewClientInfo;
    }
};
#pragma pack(pop)

class CAuthChain : public  CProtocol
{
public:
    CAuthChain();
    ~CAuthChain();

protected:
    std::vector<u_char> GetRndData(size_t BufferSize, u_char *LastHash);
    size_t GetRndDataLen(size_t BufferSize, u_char *LastHash);
    size_t RndStartPos(size_t RndLen);

protected:
    bool m_HasAuth;
    size_t m_PacketSizeMax;
    u_int m_PackId;
    u_int m_RecvId;
    std::string m_Salt;
    u_char m_LastClientHash[HMAC_HASH_BYTES];
    u_char m_LastServerHash[HMAC_HASH_BYTES];
    XorShift128Plus m_Random;
    CRC4 m_RC4Utils;
    std::vector<u_char> m_RecvBuffer;
};
#endif //SSRPLUGIN_AUTHCHAIN_H
