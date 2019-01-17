//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_COMMONPROTOCOL_H
#define SHADOWSOCKSR_CPP_COMMONPROTOCOL_H

#include <iostream>
#include <cstdint>
#include <cstring>
#include <arpa/inet.h>
#include <Plugin/Socks5/Socks5Protocol.h>

#define MAX_HOST_LENGTH 0x20

enum CCommonSocketType
{
    TCPSocketType = 0,
    UDPSocketType,
};

enum CCommonAddressType
{
    HostType = 0,
    IPv4Type,
    IPv6Type,
    UnknownType
};

struct CCommonSocketAddress
{
    struct
    {
        CCommonSocketType SocketType;
        CCommonAddressType AddressType;
    } Header;

    std::string Address;
    u_short Port;
};

struct CConnectRequest
{
    CCommonSocketAddress SocketAddress;
    void * Result;
};

inline CCommonSocketAddress ParseSocks5Address(Socks5_Connect_Request * Request, size_t Length)
{
    CCommonSocketAddress ProxyRequest = {};

    ProxyRequest.Header.SocketType = TCPSocketType;
    ProxyRequest.Header.AddressType = UnknownType;

    if (Request->Header.AddressType != SocksIPv4Type &&
        Request->Header.AddressType != SocksIPv6Type &&
        Request->Header.AddressType != SocksHostNameType)
        return ProxyRequest;

    auto & Address = Request->Address;

    char IPStr[INET6_ADDRSTRLEN] = {};

    switch (Request->Header.AddressType)
    {
        case SocksIPv4Type:
            if (Length < sizeof(Request->Header) + sizeof(Address.IPv4))
                break;

            inet_ntop(AF_INET, &Address.IPv4.IP, IPStr, INET_ADDRSTRLEN);

            ProxyRequest.Header.AddressType = IPv4Type;
            ProxyRequest.Address = IPStr;
            ProxyRequest.Port = ntohs(Address.IPv4.Port);

            break;

        case SocksIPv6Type:
            if (Length < sizeof(Request->Header) + sizeof(Address.IPv6))
                break;

            inet_ntop(AF_INET6, &Address.IPv6.IP, IPStr, INET6_ADDRSTRLEN);

            ProxyRequest.Header.AddressType = IPv6Type;
            ProxyRequest.Address = IPStr;
            ProxyRequest.Port = ntohs(Address.IPv6.Port);

            break;

        case SocksHostNameType:
            if (Address.Host.Length >= MAX_HOST_LENGTH)
                break;

            if (Length < sizeof(Request->Header) + Address.Host.Length + sizeof(Address.Host))
                break;

            ProxyRequest.Header.AddressType = HostType;
            ProxyRequest.Address.assign(Address.Host.HostName, Address.Host.HostName + Address.Host.Length);
            ProxyRequest.Port = ntohs(*(uint16_t *)(Address.Host.HostName + Address.Host.Length));

            break;
    }

    return ProxyRequest;
}

#endif //SHADOWSOCKSR_CPP_COMMONPROTOCOL_H
