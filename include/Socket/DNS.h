//
// Created by hackerl on 11/7/18.
//

#ifndef SHADOWSOCKSR_CPP_DNS_H
#define SHADOWSOCKSR_CPP_DNS_H

#include <vector>
#include <string>
#include <netdb.h>
#include <arpa/inet.h>
#include <glog/logging.h>

class CDNS
{
public:
    static std::vector<in_addr> Query(const char * HostName)
    {
        std::vector<in_addr> IPList;

        hostent * DNSInfo = gethostbyname(HostName);

        if (DNSInfo == nullptr)
        {
            LOG(ERROR) << "DNS Query Failed: " << h_errno;
            return IPList;
        }

        if (DNSInfo->h_addrtype != AF_INET || DNSInfo->h_length != sizeof(in_addr))
            return IPList;

        auto AddressPtr = (in_addr **)(DNSInfo->h_addr_list);

        for(int i = 0; AddressPtr[i] != nullptr; i ++)
            IPList.push_back(*(AddressPtr[i]));

        return IPList;
    }

    static std::vector<in_addr> QueryAdvance(const char * HostName)
    {
        std::vector<in_addr> IPList;

        addrinfo * DNSInfo;
        addrinfo Hints = {};

        Hints.ai_family = AF_INET;
        Hints.ai_socktype = SOCK_STREAM;

        int res = getaddrinfo(HostName, nullptr, &Hints, &DNSInfo);

        if (res)
        {
            LOG(ERROR) << "Advance DNS Query Failed: " << res << " Info: " << gai_strerror(res);
            return IPList;
        }

        for(auto Info = DNSInfo; Info != nullptr; Info = Info->ai_next)
            IPList.push_back(((sockaddr_in *)Info->ai_addr)->sin_addr);

        freeaddrinfo(DNSInfo);

        return IPList;
    }
};

#endif //SHADOWSOCKSR_CPP_DNS_H
