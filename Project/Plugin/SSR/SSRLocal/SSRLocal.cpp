//
// Created by hackerl on 10/28/18.
//


#include "SSRLocal.h"
#include <Utils/Binascii.h>
#include <Plugin/CommonProxy/CommonProtocol.h>
#include <Common/JSONHelper.h>
#include <Node/NodeService.h>
#include <arpa/inet.h>
#include <Plugin/Socks5/Socks5Protocol.h>

#define SOCKS5_REQUEST_SKIP 3

CSSRLocal::CSSRLocal() : m_Config()
{
    m_HasInit = false;
}

bool CSSRLocal::SetConfig(const Json::Value &Config)
{
    std::string Server = g_JSON->GetString(Config, "Server");
    u_int Port = g_JSON->GetUInt(Config, "Port");

    std::string PassWord = g_JSON->GetString(Config, "PassWord");

    if (Server.empty() || PassWord.empty())
        return false;

    m_Config.Server = Server;
    m_Config.ServerPort = u_short(Port);

    CServerInfo ServerInfo;

    CEncryptor _0_;
    _0_.Init((u_char *)PassWord.data(), PassWord.size());

    ServerInfo.Key = _0_.GetCipherKey();

    m_AuthChainLocal.SetServerInfo(ServerInfo);
    m_HTTPSimple.SetServerInfo(ServerInfo);
    m_TLSTicketAuth.SetServerInfo(ServerInfo);

    return true;
}

bool CSSRLocal::OnUpStream(const void *Buffer, size_t Length)
{
    if (!m_HasInit)
    {
        m_HasInit = true;

        size_t RequestSize = GetConnectRequestSize(Buffer, Length);

        if (RequestSize == 0)
            return false;

        std::vector<u_char> ProtocolStream = m_AuthChainLocal.ClientEncrypt((u_char *)Buffer + SOCKS5_REQUEST_SKIP, RequestSize - SOCKS5_REQUEST_SKIP);
        std::vector<u_char> ObfStream = m_TLSTicketAuth.Encode(ProtocolStream.data(), ProtocolStream.size());

        in_addr Address = {};
        inet_pton(AF_INET, "127.0.0.1", &Address);

        CConnectRequest ConnectRequest = {};

        ConnectRequest.Header.AddressType = IPv4Type;
        ConnectRequest.Header.SocketType = TCPSocketType;

        ConnectRequest.Address = m_Config.Server;
        ConnectRequest.Port = m_Config.ServerPort;

        if (!InvokeService(REQUEST_SOCKET_CONNECT, &ConnectRequest))
            return false;

        Socks5_Connect_Response Response = {};

        Response.Header.Response = uint8_t(0x00);

        return DownStream(&Response, sizeof(Response)) && UpStream(ObfStream.data(), ObfStream.size());
    }

    std::vector<u_char> ProtocolStream = m_AuthChainLocal.ClientEncrypt((u_char *)Buffer, Length);
    std::vector<u_char> ObfStream = m_TLSTicketAuth.Encode(ProtocolStream.data(), ProtocolStream.size());

    return UpStream(ObfStream.data(), ObfStream.size());
}

bool CSSRLocal::OnDownStream(const void *Buffer, size_t Length)
{
    std::vector<u_char> ProtocolStream = m_TLSTicketAuth.Decode((u_char *)Buffer, Length);
    std::vector<u_char> DataStream = m_AuthChainLocal.ClientDecrypt(ProtocolStream.data(), ProtocolStream.size());

    if (m_TLSTicketAuth.NeedSendBack())
    {
        std::vector<u_char> SendBackStream = m_TLSTicketAuth.PackSendBackData();
        UpStream(SendBackStream.data(), SendBackStream.size());
    }

    return DataStream.empty() ? true : DownStream(DataStream.data(), DataStream.size());
}
