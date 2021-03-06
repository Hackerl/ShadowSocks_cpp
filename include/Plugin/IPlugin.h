//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_IPLUGIN_H
#define SHADOWSOCKSR_CPP_IPLUGIN_H

#include <cstddef>
#include <Common/Interface.h>

class IPlugin : public Interface
{
public:
    virtual bool InitPlugin(const void *Context, size_t Size) = 0;
};

#endif //SHADOWSOCKSR_CPP_IPLUGIN_H
