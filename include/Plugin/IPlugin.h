//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_IPLUGIN_H
#define SHADOWSOCKSR_CPP_IPLUGIN_H

#include "Common/Interface.h"
#include "json/json.h"
#include "Pipe/IPipe.h"

class IPlugin : public Interface
{
public:
    virtual bool SetPipe(IPipeNode *PipeNode) = 0;
    virtual bool SetConfig(Json::Value &Config) = 0;
    virtual bool OnPipeIn(const void *Buffer, size_t Length) = 0;
    virtual bool OnDataIn(const void *Buffer, size_t Length) = 0;
};

#endif //SHADOWSOCKSR_CPP_IPLUGIN_H
