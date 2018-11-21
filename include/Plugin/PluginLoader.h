//
// Created by patteliu on 2018/11/20.
//

#ifndef SHADOWSOCKSR_CPP_PLUGINLOADER_H
#define SHADOWSOCKSR_CPP_PLUGINLOADER_H

#include "IPlugin.h"
#include <dlfcn.h>
#include <glog/logging.h>

typedef IPlugin * (* FUNC_PluginBuilder)();

class CPluginLoader
{
public:
    CPluginLoader()
    {
        m_DLHandle = nullptr;
        m_PluginBuilder = nullptr;
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

        m_PluginBuilder = (FUNC_PluginBuilder)dlsym(m_DLHandle, "NewPlugin");

        if (!m_PluginBuilder)
        {
            LOG(ERROR) << "Find NewPlugin Function Failed: " << Name;
        }
    }

    void SetConfig(const Json::Value & Config)
    {
        m_Config = Config;
    }

    IPlugin * Builder()
    {
        if (!m_PluginBuilder)
            return nullptr;

        IPlugin * Plugin =  m_PluginBuilder();

        if (!Plugin->SetConfig(m_Config))
        {
            delete Plugin;

            LOG(ERROR) << "Plugin Set Config Failed";

            return nullptr;
        }

        return Plugin;
    }

private:
    void * m_DLHandle;
    Json::Value m_Config;
    FUNC_PluginBuilder m_PluginBuilder;
};

#endif //SHADOWSOCKSR_CPP_PLUGINLOADER_H
