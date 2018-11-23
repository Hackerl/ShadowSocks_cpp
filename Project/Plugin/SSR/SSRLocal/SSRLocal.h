//
// Created by hackerl on 10/28/18.
//

#ifndef SSRPLUGIN_SSRLOCAL_H
#define SSRPLUGIN_SSRLOCAL_H

#include <Node/Node.h>
#include <Plugin/IPlugin.h>
#include "../SSRProtocol/AuthChainLocal.h"
#include "../SSRObfs/HTTPSimple.h"
#include "../SSRObfs/TLSTicketAuth.h"

struct CSSRLocalConfig
{
    std::string Server;
    u_short ServerPort;
};

class CSSRLocal : public IPlugin, public CNode
{
public:
    CSSRLocal();

public:
    bool SetConfig(const Json::Value &Config) override;

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;
    bool OnDownStream(const void *Buffer, size_t Length) override;

private:
    bool m_HasInit;
    CSSRLocalConfig m_Config;
    CAuthChainLocal m_AuthChainLocal;
    CHTTPSimple m_HTTPSimple;
    CTLSTicketAuth m_TLSTicketAuth;
};

#endif //SSRPLUGIN_SSRLOCAL_H
