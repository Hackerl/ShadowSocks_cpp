//
// Created by hackerl on 10/28/18.
//

#ifndef SSRPLUGIN_SSRLOCAL_H
#define SSRPLUGIN_SSRLOCAL_H

#include <Node/Node.h>
#include <Plugin/IPlugin.h>
#include <Protocol/IProtocol.h>

struct CSSRLocalConfig
{
    std::string Server;
    u_short ServerPort;
};

class CSSRLocal : public IPlugin, public CNode
{
public:
    CSSRLocal();
    ~CSSRLocal();

public:
    bool InitPlugin(const void *Context, size_t Size) override;

public:
    std::vector<u_char> ClientProtocolPack(const u_char * Buffer, size_t Length);
    std::vector<u_char> ClientProtocolUnPack(const u_char * Buffer, size_t Length);

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;
    bool OnDownStream(const void *Buffer, size_t Length) override;

private:
    bool m_HasInit;
    CSSRLocalConfig m_Config;
    std::vector<IProtocol *> m_ProtocolList;
};

#endif //SSRPLUGIN_SSRLOCAL_H
