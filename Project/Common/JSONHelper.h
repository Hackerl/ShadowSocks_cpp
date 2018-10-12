//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_JSONHELPER_H
#define SHADOWSOCKSR_CPP_JSONHELPER_H

#include <json/json.h>
#include <cstdio.h>
#include "Singleton.h"

class CJSONHelper {
public:
    bool HasArray(const Json::Value &jv, const char *Name);

    bool HasObject(const Json::Value &jv, const char *Name);

    std::string GetString(const Json::Value &jv, const char *Name, const char *Default = "");

    bool GetBool(const Json::Value &jv, const char *Name, bool Default = false);

    int GetInt(const Json::Value &jv, const char *Name, int Default = 0);
};

#endif //SHADOWSOCKSR_CPP_JSONHELPER_H
