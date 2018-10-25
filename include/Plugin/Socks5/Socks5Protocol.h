//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_SOCKS5PROTOCOL_H
#define SHADOWSOCKSR_CPP_SOCKS5PROTOCOL_H

#include "cstdint"
#include <netinet/in.h>

#define SOCKS5_VERSION 0x5
#define SOCKS5_NO_AUTH_METHOD 0x00
#define SOCKS5_CONNECT_COMMAND 0x01

#define  SocksIPv4Type 0x01
#define  SocksHostNameType 0x03
#define  SocksIPv6Type 0x04

#pragma pack(push, 1)

struct Socks5_Method_Request
{
    uint8_t Version;
    uint8_t NMethods;
    uint8_t Methods[0];
};

struct Socks5_Method_Response {
    uint8_t Version;
    uint8_t Method;
};

struct Socks5_Connect_Request {
    struct
    {
        uint8_t Version;
        uint8_t Command;
        uint8_t Reserved;
        uint8_t AddressType;
    } Header;

    union
    {
        struct
        {
            uint8_t Length;
            uint8_t HostName[0];
            uint16_t Port;
        } Host;

        struct
        {
            uint32_t IP;
            uint16_t Port;
        } IPv4;

        struct
        {
            in6_addr IP;
            uint16_t Port;
        } IPv6;
    } Address;
};

struct Socks5_Connect_Response {
    struct {
        uint8_t Version;
        uint8_t Response;
        uint8_t Reserved;
        uint8_t AddressType;
    } Header;

    union {
        struct {
            uint32_t IP;
            uint16_t Port;
        } IPv4;

    } Address;

    Socks5_Connect_Response() : Header(), Address()
    {
        Header.Version = SOCKS5_VERSION;
        Header.Response = 0x01;
        Header.Reserved = 0x00;
        Header.AddressType = SocksIPv4Type;
        Address.IPv4.IP = 0x00;
        Address.IPv4.Port = 0x80;
    }
};

#pragma pack(pop)

#endif //SHADOWSOCKSR_CPP_SOCKS5PROTOCOL_H
