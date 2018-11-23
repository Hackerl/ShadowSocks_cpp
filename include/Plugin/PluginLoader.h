//
// Created by patteliu on 2018/11/20.
//

#ifndef SHADOWSOCKSR_CPP_PLUGINLOADER_H
#define SHADOWSOCKSR_CPP_PLUGINLOADER_H

#include "IPlugin.h"
#include <dlfcn.h>
#include <glog/logging.h>

typedef IPlugin * (* FUNC_PluginBuilder)();

struct CPluginInfo
{
    void * DLHandle;
    FUNC_PluginBuilder PluginBuilder;
    Json::Value PluginConfig;
};

class CPluginLoader
{
#define g_PluginLoader SINGLETON_(CPluginLoader)
public:
    ~CPluginLoader()
    {
        for (auto const& PluginInfo : m_PluginInfoMap)
            dlclose(PluginInfo.second.DLHandle);

        m_PluginInfoMap.clear();
    }

public:
    void Add(const char * Name, const Json::Value & Config)
    {
        auto Iterator = m_PluginInfoMap.find(Name);

        if (Iterator != m_PluginInfoMap.end())
            return;

        void * DLHandle = dlopen(Name, RTLD_LAZY);

        if (!DLHandle)
        {
            LOG(ERROR) << "Open Library Failed: " << Name;
        }

        auto PluginBuilder = (FUNC_PluginBuilder)dlsym(DLHandle, "NewPlugin");

        if (!PluginBuilder)
        {
            LOG(ERROR) << "Find NewPlugin Function Failed: " << Name;
            return;
        }

        CPluginInfo PluginInfo = {};

        PluginInfo.DLHandle = DLHandle;
        PluginInfo.PluginBuilder = PluginBuilder;
        PluginInfo.PluginConfig = Config;

        m_PluginInfoMap.insert(std::make_pair(Name, PluginInfo));
    }

    IPlugin * Builder(const char * Name)
    {
        auto Iterator = m_PluginInfoMap.find(Name);

        if (Iterator == m_PluginInfoMap.end())
            return nullptr;

        auto & PluginInfo = Iterator->second;

        IPlugin * Plugin =  PluginInfo.PluginBuilder();

        if (!Plugin->SetConfig(PluginInfo.PluginConfig))
        {
            delete Plugin;

            LOG(ERROR) << "Plugin Set Config Failed";

            return nullptr;
        }

        return Plugin;
    }

private:
    std::map<std::string, CPluginInfo> m_PluginInfoMap;
};

#endif //SHADOWSOCKSR_CPP_PLUGINLOADER_H
