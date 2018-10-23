//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_JSONHELPER_H
#define SHADOWSOCKSR_CPP_JSONHELPER_H

#include <json/json.h>
#include <cstdio>
#include "Singleton.h"

class CJSONHelper {
#define g_JSON SINGLETON_(CJSONHelper)
public:
    bool HasArray(const Json::Value &jv, const char *Name)
    {
        if (!jv.isObject())
            return false;

        if (!jv.isMember(Name))
            return false;

        if (!jv[Name].isArray())
            return false;

        return true;
    }

    bool HasObject(const Json::Value &jv, const char *Name)
    {
        if (!jv.isObject())
            return false;

        if (!jv.isMember(Name))
            return false;

        if (!jv[Name].isObject())
            return false;

        return true;
    }


    std::string GetString(const Json::Value &jv, const char *Name, const char *Default = "")
    {
        if (!jv.isObject())
            return Default;

        if (!jv.isMember(Name))
            return Default;

        if (jv[Name].isString())
            return jv[Name].asString();

        if (jv[Name].isInt())
        {
            char data[32] = {};
            snprintf(data, 32, "%d", jv[Name].asInt());
            return data;
        }

        return Default;
    }

    bool GetBool(const Json::Value &jv, const char *Name, bool Default = false)
    {
        if (!jv.isObject())
            return Default;

        if (!jv.isMember(Name))
            return Default;

        return jv[Name].asBool();
    }

    int GetInt(const Json::Value &jv, const char *Name, int Default = 0)
    {
        if (!jv.isObject())
            return Default;

        if (!jv.isMember(Name))
            return Default;

        if (!jv[Name].isInt())
            return Default;

        return jv[Name].asInt();
    }

    unsigned int GetUInt(const Json::Value &jv, const char *Name, int Default = 0)
    {
        if (!jv.isObject())
            return Default;

        if (!jv.isMember(Name))
            return Default;

        if (!jv[Name].isUInt())
            return Default;

        return jv[Name].asUInt();
    }
};

#endif //SHADOWSOCKSR_CPP_JSONHELPER_H