//
// Created by patteliu on 2018/11/23.
//

#ifndef SHADOWSOCKSR_CPP_PROTOCOLLOADER_H
#define SHADOWSOCKSR_CPP_PROTOCOLLOADER_H

#include "IProtocol.h"
#include <map>
#include <dlfcn.h>
#include <fstream>
#include <glog/logging.h>
#include <Common/JSONHelper.h>

typedef IProtocol * (* FUNC_ProtocolBuilder)(const char * ProtocolName);

struct CProtocolInfo
{
    void * DLHandle;
    FUNC_ProtocolBuilder ProtocolBuilder;
};

class CProtocolLoader
{
#define g_ProtocolLoader SINGLETON_(CProtocolLoader)
public:
    CProtocolLoader()
    {
        ReadConfig();
    }

    ~CProtocolLoader()
    {
        for (auto const& PluginInfo : m_ProtocolInfoMap)
            dlclose(PluginInfo.second.DLHandle);

        m_ProtocolInfoMap.clear();
    }

public:
    void ReadConfig()
    {
        std::ifstream ConfigFile("Protocol.json");

        if (!ConfigFile.is_open())
        {
            LOG(ERROR) << "Cant Not Open Protocol Config File";
        }

        Json::Value Config;
        Json::CharReaderBuilder Builder;
        std::string Error;

        if (!Json::parseFromStream(Builder, ConfigFile, &Config, &Error))
        {
            LOG(ERROR) << "Cant Not Parse Protocol Config File: " << Error;
            return;
        }

        m_ProtocolConfig = Config;
    }

public:
    void Add(const char * Name)
    {
        auto Iterator = m_ProtocolInfoMap.find(Name);

        if (Iterator != m_ProtocolInfoMap.end())
            return;

        std::string LibraryPath = g_JSON->GetString(m_ProtocolConfig, Name);

        if (LibraryPath.empty())
        {
            LOG(ERROR) << "Cant Not Find Protocol Library Path: " << Name;
            return;
        }

        void * DLHandle = dlopen(LibraryPath.c_str(), RTLD_LAZY);

        if (!DLHandle)
        {
            LOG(ERROR) << "Open Library Failed: " << LibraryPath;
            return;
        }

        auto ProtocolBuilder = (FUNC_ProtocolBuilder)dlsym(DLHandle, "NewProtocol");

        if (!ProtocolBuilder)
        {
            LOG(ERROR) << "Find NewProtocol Function Failed: " << Name;
            return;
        }

        CProtocolInfo ProtocolInfo = {};

        ProtocolInfo.DLHandle = DLHandle;
        ProtocolInfo.ProtocolBuilder = ProtocolBuilder;

        m_ProtocolInfoMap.insert(std::make_pair(Name, ProtocolInfo));
    }

    IProtocol * Builder(const char * Name)
    {
        auto Iterator = m_ProtocolInfoMap.find(Name);

        if (Iterator == m_ProtocolInfoMap.end())
            return nullptr;

        auto & ProtocolInfo = Iterator->second;

        IProtocol * Protocol = ProtocolInfo.ProtocolBuilder(Name);

        if (!Protocol)
        {
            LOG(ERROR) << "Protocol Builder Failed: " << Name;
            return nullptr;
        }

        return Protocol;
    }

private:
    Json::Value m_ProtocolConfig;
    std::map<std::string, CProtocolInfo> m_ProtocolInfoMap;
};

#endif //SHADOWSOCKSR_CPP_PROTOCOLLOADER_H
