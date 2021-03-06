//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_SOCKSCLIENT_H
#define SHADOWSOCKSR_CPP_SOCKSCLIENT_H

#include <Node/Node.h>
#include <Plugin/IPlugin.h>

enum CSock5Status
{
    MethodRequestStage = 0,
    ConnectRequestStage,
    ConnectSuccessStage
};

class CSocks5Proxy : public IPlugin, public CNode
{
public:
    CSocks5Proxy();

public:
    bool InitPlugin(const void *Context, size_t Size) override;

public:
    bool OnUpStream(const void *Buffer, size_t Length) override;

public:
    bool MethodRequestHandler(const void *Buffer, size_t Length);
    bool ConnectRequestHandler(const void *Buffer, size_t Length);

private:
    CSock5Status m_Status;
};

#endif //SHADOWSOCKSR_CPP_SOCKSCLIENT_H
