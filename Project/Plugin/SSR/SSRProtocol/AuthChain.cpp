//
// Created by hackerl on 10/21/18.
//

#include "AuthChain.h"
#include <Utils/Random.h>

CAuthChain::CAuthChain() : m_LastClientHash(), m_LastServerHash()
{
    m_HasAuth = false;
    m_PacketSizeMax = 2800;
    m_OverHead = 4;
    m_PackId = 1;
    m_RecvId = 1;
    m_Salt = "auth_chain_a";
}

CAuthChain::~CAuthChain()
{
    m_RecvBuffer.clear();
}

size_t CAuthChain::GetRndDataLen(size_t BufferSize, u_char *LastHash)
{
    if (BufferSize > 1440)
        return 0;

    m_Random.InitFromBinLen(LastHash, HMAC_HASH_BYTES, BufferSize);

    if (BufferSize > 1300)
        return m_Random.Next() % 31;

    if (BufferSize > 900)
        return m_Random.Next() % 127;

    if (BufferSize > 400)
        return m_Random.Next() % 521;

    return m_Random.Next() % 1021;
}

size_t CAuthChain::RndStartPos(size_t RndLen)
{
    return RndLen > 0 ? m_Random.Next() % 8589934609 % RndLen : 0;
}

std::vector<u_char> CAuthChain::GetRndData(size_t BufferSize, u_char *LastHash)
{
    std::vector<u_char> RndDataStream;

    size_t RndDataLen = GetRndDataLen(BufferSize, LastHash);

    for (int i = 0; i < RndDataLen; i ++)
        RndDataStream.push_back(u_char(g_Random->Next() % 256));

    return RndDataStream;
}
