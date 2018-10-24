//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_PLUGIN_H
#define SHADOWSOCKSR_CPP_PLUGIN_H

#include "IPlugin.h"

class CPlugin : public IPlugin
{
public:
    CPlugin()
    {
        m_PipeNode = nullptr;
    }

public:
    bool SetConfig(Json::Value &Config) override
    {
        return true;
    }

    bool NodeInit(void * args)
    {
        if (m_PipeNode == nullptr)
            return false;

        m_PipeNode->PipeInit(args);
    }

    bool SetPipe(IPipeNode *PipeNode) override
    {
        m_PipeNode = PipeNode;
    }

    bool OnPipeIn(const void *Buffer, size_t Length) override
    {
        if (m_PipeNode == nullptr)
            return false;

        return m_PipeNode->PipeOut(Buffer, Length);
    }

    bool OnDataIn(const void *Buffer, size_t Length) override
    {
        if (m_PipeNode == nullptr)
            return false;

        return m_PipeNode->PipeIn(Buffer, Length);
    }

    bool Destroy()
    {
        if (m_PipeNode == nullptr)
            return false;

        m_PipeNode->PipeClose();
    }

protected:
    IPipeNode * m_PipeNode;
};

#endif //SHADOWSOCKSR_CPP_PLUGIN_H
