//
// Created by hackerl on 10/28/18.
//

#ifndef SSRPLUGIN_HTTPSIMPLE_H
#define SSRPLUGIN_HTTPSIMPLE_H

#include <vector>
#include <sys/types.h>
#include <Protocol/Protocol.h>

class CHTTPSimple : public CProtocol
{
public:
    CHTTPSimple();

public:
    std::string GetHTTPHeader(const u_char *Buffer, size_t Length);

public:
    std::vector<u_char> ClientPack(const u_char *Buffer, size_t Length) override;
    std::vector<u_char> ClientUnPack(const u_char *Buffer, size_t Length) override;

private:
    bool m_HasRecvHeader;
    bool m_HasSendHeader;
};


#endif //SSRPLUGIN_HTTPSIMPLE_H
