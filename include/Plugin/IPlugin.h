//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_IPLUGIN_H
#define SHADOWSOCKSR_CPP_IPLUGIN_H

#include <Common/Interface.h>
#include <json/json.h>

class IPlugin : public Interface
{
public:
    virtual bool SetConfig(const Json::Value &Config) = 0;
};

#endif //SHADOWSOCKSR_CPP_IPLUGIN_H
