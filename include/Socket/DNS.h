//
// Created by hackerl on 11/7/18.
//

#ifndef SHADOWSOCKSR_CPP_DNS_H
#define SHADOWSOCKSR_CPP_DNS_H

#include <vector>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>

class CDNS
{
public:
    static std::vector<in_addr> Query(const char * HostName)
    {
        std::vector<in_addr> IPList;

        hostent * DNSInfo = gethostbyname(HostName);

        if (DNSInfo == nullptr)
            return IPList;

        if (DNSInfo->h_addrtype != AF_INET || DNSInfo->h_length != sizeof(in_addr))
            return IPList;

        auto AddressPtr = (in_addr **)(DNSInfo->h_addr_list);

        for(int i = 0; AddressPtr[i] != nullptr; i ++)
            IPList.push_back(*(AddressPtr[i]));

        return IPList;
    }
};

#endif //SHADOWSOCKSR_CPP_DNS_H
