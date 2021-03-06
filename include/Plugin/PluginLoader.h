//
// Created by patteliu on 2018/11/20.
//

#ifndef SHADOWSOCKSR_CPP_PLUGINLOADER_H
#define SHADOWSOCKSR_CPP_PLUGINLOADER_H

#include "IPlugin.h"
#include <dlfcn.h>
#include <glog/logging.h>
#include <json/json.h>

typedef IPlugin * (* FUNC_PluginBuilder)();

struct CPluginInfo
{
    void * DLHandle;
    FUNC_PluginBuilder PluginBuilder;
    std::string PluginConfig;
};

class CPluginLoader
{
#define g_PluginLoader SINGLETON_(CPluginLoader)
public:
    ~CPluginLoader()
    {
        for (auto const& PluginInfo : m_ProtocolInfoMap)
            dlclose(PluginInfo.second.DLHandle);

        m_ProtocolInfoMap.clear();
    }

public:
    void Add(const char * Name, const Json::Value & Config)
    {
        auto Iterator = m_ProtocolInfoMap.find(Name);

        if (Iterator != m_ProtocolInfoMap.end())
            return;

        void * DLHandle = dlopen(Name, RTLD_LAZY);

        if (!DLHandle)
        {
            LOG(ERROR) << "Open Library Failed: " << Name;
            return;
        }

        auto PluginBuilder = (FUNC_PluginBuilder)dlsym(DLHandle, "NewPlugin");

        if (!PluginBuilder)
        {
            LOG(ERROR) << "Find NewPlugin Function Failed: " << Name;
            return;
        }

        Json::FastWriter Writer;
        CPluginInfo PluginInfo = {};

        PluginInfo.DLHandle = DLHandle;
        PluginInfo.PluginBuilder = PluginBuilder;
        PluginInfo.PluginConfig = Writer.write(Config);

        m_ProtocolInfoMap.insert(std::make_pair(Name, PluginInfo));
    }

    IPlugin * Builder(const char * Name)
    {
        auto Iterator = m_ProtocolInfoMap.find(Name);

        if (Iterator == m_ProtocolInfoMap.end())
            return nullptr;

        auto & PluginInfo = Iterator->second;

        IPlugin * Plugin =  PluginInfo.PluginBuilder();

        if (!Plugin)
        {
            LOG(ERROR) << "Plugin Builder Failed";
            return nullptr;
        }

        if (!Plugin->InitPlugin(PluginInfo.PluginConfig.data(), PluginInfo.PluginConfig.size()))
        {
            delete Plugin;

            LOG(ERROR) << "Plugin Set Config Failed";

            return nullptr;
        }

        return Plugin;
    }

private:
    std::map<std::string, CPluginInfo> m_ProtocolInfoMap;
};

#endif //SHADOWSOCKSR_CPP_PLUGINLOADER_H
