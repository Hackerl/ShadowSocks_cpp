//
// Created by patteliu on 2018/11/20.
//

#ifndef SHADOWSOCKSR_CPP_PLUGINLOADER_H
#define SHADOWSOCKSR_CPP_PLUGINLOADER_H

#include <dlfcn.h>
#include <glog/logging.h>
#include <Node/INode.h>

typedef INode * (* FUNC_NodeBuilder)();

class CPluginLoader
{
public:
    CPluginLoader()
    {
        m_DLHandle = nullptr;
        m_NodeBuilder = nullptr;
    }

    ~CPluginLoader()
    {
        if (m_DLHandle != nullptr)
            dlclose(m_DLHandle);
    }

public:
    void Init(const char * Name)
    {
        void * m_DLHandle = dlopen(Name, RTLD_LAZY);

        if (!m_DLHandle)
        {
            LOG(ERROR) << "Open Library Failed: " << Name;
        }

        m_NodeBuilder = (FUNC_NodeBuilder)dlsym(m_DLHandle, "NewPlugin");

        if (!m_NodeBuilder)
        {
            LOG(ERROR) << "Find NewPlugin Function Failed: " << Name;
        }
    }

    void SetConfig(const Json::Value & Config)
    {
        m_Config = Config;
    }

    INode * Builder()
    {
        if (!m_NodeBuilder)
            return nullptr;

        INode * Node =  m_NodeBuilder();

        if (!Node->SetNodeConfig(m_Config))
        {
            delete Node;

            LOG(ERROR) << "Node Set Config Failed";

            return nullptr;
        }

        return Node;
    }

private:
    void * m_DLHandle;
    Json::Value m_Config;
    FUNC_NodeBuilder m_NodeBuilder;
};

#endif //SHADOWSOCKSR_CPP_PLUGINLOADER_H
