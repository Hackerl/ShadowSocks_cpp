//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_COMMONPROTOCOL_H
#define SHADOWSOCKSR_CPP_COMMONPROTOCOL_H

#include <cstdint>
#include <cstring>
#include "Plugin/Socks5/Socks5Protocol.h"

#define MAX_HOST_LENGTH 0x20

enum CCommonProxyType
{
    Socks5ProxyType = 0,
    HTTPTunnelType,
};

enum CCommonAddressType
{
    HostType = 0,
    IPv4Type,
    IPv6Type,
    UnknownType
};

#pragma pack(push, 1)
struct CCommonProxyRequest
{
    struct
    {
        CCommonProxyType ProxyType;
        CCommonAddressType AddressType;
    } Header;

    uint16_t Port;

    union
    {
        uint32_t IPv4Address;
        in6_addr IPv6Address;
        uint8_t HostName[MAX_HOST_LENGTH];
    };
};
#pragma pack(pop)

inline CCommonProxyRequest ParseSocks5Address(Socks5_Connect_Request * Request, size_t Length)
{
    CCommonProxyRequest ProxyRequest = {};

    ProxyRequest.Header.ProxyType = Socks5ProxyType;
    ProxyRequest.Header.AddressType = UnknownType;

    if (Request->Header.AddressType != SocksIPv4Type &&
        Request->Header.AddressType != SocksIPv6Type &&
        Request->Header.AddressType != SocksHostNameType)
        return ProxyRequest;

    auto & Address = Request->Address;

    switch (Request->Header.AddressType)
    {
        case SocksIPv4Type:
            if (Length < sizeof(Request->Header) + sizeof(Address.IPv4))
                break;

            ProxyRequest.Header.AddressType =  IPv4Type;
            ProxyRequest.IPv4Address = Address.IPv4.IP;
            ProxyRequest.Port = Address.IPv4.Port;

            break;

        case SocksHostNameType:
            if (Address.Host.Length >= MAX_HOST_LENGTH)
                break;

            if (Length < sizeof(Request->Header) + Address.Host.Length + sizeof(Address.Host))
                break;

            ProxyRequest.Header.AddressType = HostType;

            memcpy(ProxyRequest.HostName, Address.Host.HostName, Address.Host.Length);
            ProxyRequest.Port = * (uint16_t *)(Address.Host.HostName + Address.Host.Length);

            break;
    }

    return ProxyRequest;
}

#endif //SHADOWSOCKSR_CPP_COMMONPROTOCOL_H
