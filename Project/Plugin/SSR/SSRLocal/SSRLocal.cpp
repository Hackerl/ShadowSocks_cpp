//
// Created by hackerl on 10/28/18.
//

#include "SSRLocal.h"
#include <arpa/inet.h>
#include <Common/JSONHelper.h>
#include <Node/NodeService.h>
#include <Utils/Encryptor.h>
#include <Protocol/ProtocolLoader.h>
#include <Plugin/Socks5/Socks5Protocol.h>
#include <Plugin/CommonProxy/CommonProtocol.h>

#define SOCKS5_REQUEST_SKIP 3

CSSRLocal::CSSRLocal() : m_Config()
{
    m_HasInit = false;
}

CSSRLocal::~CSSRLocal()
{
    for (auto const & Protocol : m_ProtocolList)
        delete Protocol;

    m_ProtocolList.clear();
}

bool CSSRLocal::InitPlugin(const void *Context, size_t Size)
{
    Json::Value Config;
    std::string Error;

    if (!ParseJsonFromData((const char *)Context, Size, Config, Error))
        return false;

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

    if (!g_JSON->HasArray(Config, "Protocols"))
        return false;

    Json::Value ProtocolNames = Config["Protocols"];

    for (auto const & ProtocolName : ProtocolNames)
    {
        std::string ProtocolNameStr = ProtocolName.asString();

        g_ProtocolLoader->Add(ProtocolNameStr.c_str());

        IProtocol * Protocol = g_ProtocolLoader->Builder(ProtocolNameStr.c_str());

        if (!Protocol)
            continue;

        m_ProtocolList.push_back(Protocol);
    }

    for (auto const & Protocol : m_ProtocolList)
        Protocol->SetServerInfo(ServerInfo);

    return true;
}

std::vector<u_char> CSSRLocal::ClientProtocolPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> PackStream(Buffer, Buffer + Length);

    for (auto const & Protocol : m_ProtocolList)
        PackStream = Protocol->ClientPack(PackStream.data(), PackStream.size());

    return PackStream;
}

std::vector<u_char> CSSRLocal::ClientProtocolUnPack(const u_char *Buffer, size_t Length)
{
    std::vector<u_char> UnPackStream(Buffer, Buffer + Length);

    for (std::vector<IProtocol *>::reverse_iterator it = m_ProtocolList.rbegin(); it != m_ProtocolList.rend(); it ++)
    {
        auto & Protocol = * it;

        UnPackStream = Protocol->ClientUnPack(UnPackStream.data(), UnPackStream.size());

        if (!Protocol->NeedSendBack())
            continue;

        std::vector<u_char> SendBackStream = Protocol->PackSendBackData();

        UpStream(SendBackStream.data(), SendBackStream.size());
    }

    return UnPackStream;
}

bool CSSRLocal::OnUpStream(const void *Buffer, size_t Length)
{
    if (!m_HasInit)
    {
        m_HasInit = true;

        size_t RequestSize = GetConnectRequestSize(Buffer, Length);

        if (RequestSize == 0)
            return false;

        std::vector<u_char> PackStream = ClientProtocolPack((const u_char *)Buffer + SOCKS5_REQUEST_SKIP, RequestSize - SOCKS5_REQUEST_SKIP);

        CConnectRequest ConnectRequest = {};

        ConnectRequest.Header.AddressType = IPv4Type;
        ConnectRequest.Header.SocketType = TCPSocketType;

        ConnectRequest.Address = m_Config.Server;
        ConnectRequest.Port = m_Config.ServerPort;

        if (!InvokeService(REQUEST_SOCKET_CONNECT, &ConnectRequest))
            return false;

        Socks5_Connect_Response Response = {};

        Response.Header.Response = uint8_t(0x00);

        return DownStream(&Response, sizeof(Response)) && UpStream(PackStream.data(), PackStream.size());
    }

    std::vector<u_char> PackStream = ClientProtocolPack((const u_char *)Buffer, Length);

    return UpStream(PackStream.data(), PackStream.size());
}

bool CSSRLocal::OnDownStream(const void *Buffer, size_t Length)
{
    std::vector<u_char> UnPackStream = ClientProtocolUnPack((const u_char *)Buffer, Length);

    return UnPackStream.empty() ? true : DownStream(UnPackStream.data(), UnPackStream.size());
}
