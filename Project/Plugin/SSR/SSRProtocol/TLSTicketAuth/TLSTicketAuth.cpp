//
// Created by hackerl on 11/4/18.
//

#include "TLSTicketAuth.h"
#include <Utils/HMAC.h>
#include <Common/Utils/Binascii.h>
#include <Common/Utils/VectorHelper.h>
#include <algorithm>

static u_char TLSVersion[2] = {0x03, 0x03};

CTLSTicketAuth::CTLSTicketAuth()
{
    m_Status = 0;
    m_OverHead = 5;
    m_NeedSendBack = false;
}

CTLSTicketAuth::~CTLSTicketAuth()
{
    m_RecvBuffer.clear();
    m_SendBuffer.clear();
}

bool CTLSTicketAuth::NeedSendBack()
{
    return m_NeedSendBack;
}

std::vector<u_char> CTLSTicketAuth::PackSendBackData()
{
    std::vector<u_char> SendBackStream;

    if (m_Status == 1 && m_NeedSendBack)
    {
        SendBackStream.push_back(0x14);
        VectorAppend(SendBackStream, TLSVersion, sizeof(TLSVersion));
        VectorAppend(SendBackStream, CBinascii::UnHexlify("000101"));

        SendBackStream.push_back(0x16);
        VectorAppend(SendBackStream, TLSVersion, sizeof(TLSVersion));
        VectorAppend(SendBackStream, CBinascii::UnHexlify("0020"));

        for (int i = 0; i < 22; i ++)
            SendBackStream.push_back(u_char(g_Random->Next()));

        std::vector<u_char> HMACKey;
        std::vector<u_char> ClientID = g_TLSAuthSession->GetClientID();

        VectorAppend(HMACKey, m_ServerInfo.Key);
        VectorAppend(HMACKey, ClientID);

        u_char DataHMACSha1[HMAC_HASH_BYTES] = {};
        CHMAC::Sha1Hash(DataHMACSha1, SendBackStream.data(), SendBackStream.size(), HMACKey.data(), HMACKey.size());

        VectorAppend(SendBackStream, DataHMACSha1, 10);
        VectorAppend(SendBackStream, m_SendBuffer);

        m_SendBuffer.clear();
        m_SendBuffer.shrink_to_fit();

        m_Status = 8;
        m_NeedSendBack = false;
    }

    return SendBackStream;
}

std::vector<u_char> CTLSTicketAuth::SNI(std::string Host)
{
    std::vector<u_char> DataStream;

    DataStream.push_back(0x00);

    u_short HostLen = htons(u_short(Host.size()));
    VectorAppendData(DataStream, HostLen);
    VectorAppendStr(DataStream, Host);

    std::vector<u_char> TMPStream;

    TMPStream.push_back(0x00);
    TMPStream.push_back(0x00);

    VectorAppendData(TMPStream, htons(u_short(DataStream.size())) + 2);
    VectorAppendData(TMPStream, htons(u_short(DataStream.size())));

    VectorPrepend(DataStream, TMPStream);

    return DataStream;
}

std::vector<u_char> CTLSTicketAuth::PackAuthData()
{
    std::vector<u_char> AuthData;

    auto UTC = (u_int)time(nullptr);
    VectorAppendData(AuthData, htonl(UTC));

    for (int i = 0; i < 18; i ++)
        AuthData.push_back(u_char(g_Random->Next() % 256));

    std::vector<u_char> HMACKey;
    std::vector<u_char> ClientID = g_TLSAuthSession->GetClientID();

    VectorAppend(HMACKey, m_ServerInfo.Key);
    VectorAppend(HMACKey, ClientID);

    u_char DataHMACSha1[HMAC_HASH_BYTES] = {};
    CHMAC::Sha1Hash(DataHMACSha1, AuthData.data(), AuthData.size(), HMACKey.data(), HMACKey.size());

    VectorAppend(AuthData, DataHMACSha1, 10);

    return AuthData;
}

std::vector<u_char> CTLSTicketAuth::ClientPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> EncodeStream;

    if (m_Status == -1)
    {
        EncodeStream.insert(EncodeStream.end(), Buffer, Buffer + Length);
        return EncodeStream;
    }

    if (m_Status == 8)
    {
        size_t Index = 0;

        while (Index < Length)
        {
            size_t RandomSize = std::min(g_Random->Next() % 4096 + 100, 2048U);

            size_t PackSize = std::min(RandomSize, Length - Index);

            EncodeStream.push_back(0x17);

            VectorAppend(EncodeStream, TLSVersion, sizeof(TLSVersion));
            VectorAppendData(EncodeStream, htons(PackSize));
            VectorAppend(EncodeStream, Buffer + Index, PackSize);

            Index += PackSize;
        }

        return EncodeStream;
    }

    if (Length > 0)
    {
        m_SendBuffer.push_back(0x17);
        VectorAppend(m_SendBuffer, TLSVersion, sizeof(TLSVersion));

        VectorAppendData(m_SendBuffer, htons(Length));
        VectorAppend(m_SendBuffer, Buffer, Length);
    }

    if (m_Status == 0)
    {
        m_Status = 1;

        VectorAppend(EncodeStream, TLSVersion, sizeof(TLSVersion));
        VectorAppend(EncodeStream, PackAuthData());

        EncodeStream.push_back(0x20);

        VectorAppend(EncodeStream, g_TLSAuthSession->GetClientID());
        VectorAppend(EncodeStream, CBinascii::UnHexlify("001cc02bc02fcca9cca8cc14cc13c00ac014c009c013009c0035002f000a0100"));

        std::vector<u_char> ExtendData;

        VectorAppend(ExtendData, CBinascii::UnHexlify("ff01000100"));
        VectorAppend(ExtendData, SNI("bing.com"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("00170000"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("0023"));

        u_short TicketBufLen = u_short(g_TLSAuthSession->GetTicketBuf().size());
        VectorAppendData(ExtendData, htons(TicketBufLen));

        VectorAppend(ExtendData, g_TLSAuthSession->GetTicketBuf());

        VectorAppend(ExtendData, CBinascii::UnHexlify("000d001600140601060305010503040104030301030302010203"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("000500050100000000"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("00120000"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("75500000"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("000b00020100"));
        VectorAppend(ExtendData, CBinascii::UnHexlify("000a0006000400170018"));

        auto ExtendDataLen = u_short(ExtendData.size());

        VectorAppendData(EncodeStream, htons(ExtendDataLen));
        VectorAppend(EncodeStream, ExtendData);

        std::vector<u_char> TMPStream;

        VectorAppend(TMPStream, CBinascii::UnHexlify("0100"));
        VectorAppendData(TMPStream, htons(u_short(EncodeStream.size())));

        VectorPrepend(EncodeStream, TMPStream);

        TMPStream.clear();

        VectorAppend(TMPStream, CBinascii::UnHexlify("160301"));
        VectorAppendData(TMPStream, htons(u_short(EncodeStream.size())));

        VectorPrepend(EncodeStream, TMPStream);
    }

    return EncodeStream;
}

std::vector<u_char> CTLSTicketAuth::ClientUnPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> DecodeStream;

    if (m_Status == -1)
    {
        DecodeStream.insert(DecodeStream.end(), Buffer, Buffer + Length);
        return DecodeStream;
    }

    if (m_Status == 8)
    {
        m_RecvBuffer.insert(m_RecvBuffer.end(), Buffer, Buffer + Length);

        while (m_RecvBuffer.size() > m_OverHead)
        {
            u_short DataSize = ntohs(*(u_short *)(m_RecvBuffer.data() + 3));

            if (m_RecvBuffer.size() < DataSize + m_OverHead)
                break;

            VectorAppend(DecodeStream, m_RecvBuffer.data() + m_OverHead, DataSize);
            m_RecvBuffer.erase(m_RecvBuffer.begin(), m_RecvBuffer.begin() + DataSize + m_OverHead);
        }

        m_RecvBuffer.shrink_to_fit();

        return DecodeStream;
    }

    m_NeedSendBack = true;

    return DecodeStream;
}
