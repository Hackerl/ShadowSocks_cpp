//
// Created by hackerl on 10/21/18.
//

#include "AuthChainLocal.h"
#include <Utils/Base64.h>
#include <Utils/RC4.h>
#include <Utils/Vector.h>
#include <cstring>
#include <algorithm>

CAuthHeader CAuthChainLocal::PackAuthData()
{
    CAuthHeader AuthHeader;

    std::vector<u_char> HeadHMACKey;

    VectorAppend(HeadHMACKey, m_ServerInfo.IV);
    VectorAppend(HeadHMACKey, m_ServerInfo.Key);

    AuthHeader.CheckHead = g_Random->Next();

    u_char CheckHeadHMACMd5[HMAC_HASH_BYTES] = {};
    CHMAC::Md5Hash(CheckHeadHMACMd5, (u_char *) &AuthHeader.CheckHead,
            sizeof(int), HeadHMACKey.data(), HeadHMACKey.size());

    memcpy(m_LastClientHash, CheckHeadHMACMd5, HMAC_HASH_BYTES);
    memcpy(AuthHeader.HeadHMACMd5, CheckHeadHMACMd5, sizeof(AuthHeader.HeadHMACMd5));

    g_AuthChainSession->LockSession();

    u_int ClientID = g_AuthChainSession->GetClientID();
    u_int ConnectionID = g_AuthChainSession->GetConnectionID();

    if (ClientID == 0 || ConnectionID == 0 || ConnectionID > 0xff000000)
    {
        ClientID = g_Random->Next();
        ConnectionID = g_Random->Next() & 0x00ffffff;

        g_AuthChainSession->SetClientID(ClientID);
    }

    g_AuthChainSession->SetConnectionID(ConnectionID + 1);
    g_AuthChainSession->UnLockSession();

    AuthHeader.ClientInfo.SetInfo(ClientID, ConnectionID, m_OverHead);

    AuthHeader.UserID = m_UserID ^ * (u_int *)(m_LastClientHash + 8);

    std::vector<u_char> AESKey;

    std::string UserKeyBase64 = Base64::encode(m_ServerInfo.Key.data(), m_ServerInfo.Key.size());

    VectorAppendStr(AESKey, UserKeyBase64);
    VectorAppendStr(AESKey, m_Salt);

    AuthHeader.EncryptInfo(AESKey);

    u_char InfoHMACMd5[HMAC_HASH_BYTES] = {};
    CHMAC::Md5Hash(InfoHMACMd5, (u_char *) &AuthHeader.UserID,
            sizeof(AuthHeader.UserID) + sizeof(AuthHeader.ClientInfo),
            m_ServerInfo.Key.data(), m_ServerInfo.Key.size());

    memcpy(m_LastServerHash, InfoHMACMd5, HMAC_HASH_BYTES);
    memcpy(AuthHeader.InfoHMACMd5, InfoHMACMd5, sizeof(AuthHeader.InfoHMACMd5));

    //Init RC4 Key
    std::string RC4Key = Base64::encode(m_ServerInfo.Key.data(), m_ServerInfo.Key.size())
                         + Base64::encode(m_LastClientHash, sizeof(m_LastClientHash));

    m_RC4Utils.Init((u_char *)RC4Key.c_str(), RC4Key.size());

    return AuthHeader;
}

std::vector<u_char> CAuthChainLocal::PackClientData(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> CipherStream = m_RC4Utils.Encrypt(Buffer, Length);
    size_t RealCipherSize = CipherStream.size();

    std::vector<u_char> RndDataStream = GetRndData(RealCipherSize, m_LastClientHash);
    size_t RndDataSize = RndDataStream.size();

    if (!RndDataStream.empty())
    {
        size_t StartPos = RndStartPos(RndDataSize);

        VectorPrepend(CipherStream, RndDataStream, StartPos);
        VectorAppend(CipherStream, RndDataStream.data() + StartPos, RndDataSize - StartPos);
    }

    u_short PackLen = (u_short)RealCipherSize ^ *(u_short *)(m_LastClientHash + 14);

    VectorPrependData(CipherStream, PackLen);

    std::vector<u_char> HMACKey;

    VectorAppend(HMACKey, m_ServerInfo.Key);
    VectorAppendData(HMACKey, m_PackId);

    u_char DataHMACMd5[HMAC_HASH_BYTES] = {};
    CHMAC::Md5Hash(DataHMACMd5, CipherStream.data(), CipherStream.size(), HMACKey.data(), HMACKey.size());

    memcpy(m_LastClientHash, DataHMACMd5, HMAC_HASH_BYTES);

    VectorAppend(CipherStream, DataHMACMd5, 2);

    m_PackId ++;

    return CipherStream;
}

std::vector<u_char> CAuthChainLocal::ClientPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> CipherStream;

    if (!m_HasAuth)
    {
        m_HasAuth = true;

        CAuthHeader AuthHeader = PackAuthData();

        VectorPrependData(CipherStream, AuthHeader);
    }

    size_t Index = 0;

    while (Index < Length)
    {
        size_t PackSize = std::min(m_PacketSizeMax, Length - Index);

        std::vector<u_char> PacketStream = PackClientData(Buffer + Index, PackSize);
        VectorAppend(CipherStream, PacketStream);

        Index += PackSize;
    }

    return CipherStream;
}

std::vector<u_char> CAuthChainLocal::ClientUnPack(const u_char *Buffer, size_t Length)
{
    VectorAppend(m_RecvBuffer, Buffer, Length);

    std::vector<u_char> RawDataStream;

    while (m_RecvBuffer.size() > m_OverHead)
    {
        u_short RealCipherSize = * (u_short *) m_RecvBuffer.data() ^ *(u_short *)(m_LastServerHash + 14);

        //TODO Split CLient Server Random if muti-thread
        size_t RndDataLen = GetRndDataLen(RealCipherSize, m_LastServerHash);

        size_t DataLen = RealCipherSize + RndDataLen;

        if (DataLen + m_OverHead > m_RecvBuffer.size())
            break;

        std::vector<u_char> HMACKey;

        VectorAppend(HMACKey, m_ServerInfo.Key);
        VectorAppendData(HMACKey, m_RecvId);

        u_char DataHMACMd5[HMAC_HASH_BYTES] = {};
        CHMAC::Md5Hash(DataHMACMd5, m_RecvBuffer.data(), DataLen + sizeof(RealCipherSize), HMACKey.data(), HMACKey.size());

        if (*(u_short *)DataHMACMd5 != *(u_short *)(m_RecvBuffer.data() + DataLen + sizeof(RealCipherSize)))
            break;

        memcpy(m_LastServerHash, DataHMACMd5, HMAC_HASH_BYTES);

        size_t StartPos = RndStartPos(RndDataLen) + sizeof(RealCipherSize);

        std::vector<u_char> DecipherStream = m_RC4Utils.Decrypt(m_RecvBuffer.data() + StartPos, RealCipherSize);

        if (m_RecvId == 1)
        {
            m_ServerInfo.TCPMss = * (u_short *) DecipherStream.data();
            DecipherStream.erase(DecipherStream.begin(), DecipherStream.begin() + sizeof(m_ServerInfo.TCPMss));
        }

        m_RecvId ++;

        m_RecvBuffer.erase(m_RecvBuffer.begin(), m_RecvBuffer.begin() + DataLen + m_OverHead);

        if (!DecipherStream.empty())
            VectorAppend(RawDataStream, DecipherStream);
    }

    m_RecvBuffer.shrink_to_fit();

    return RawDataStream;
}
