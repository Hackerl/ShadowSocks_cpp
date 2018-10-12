//
// Created by patteliu on 2018/10/12.
//

#include "JSONHelper.h"

bool CJSONHelper::HasArray(const Json::Value& jv, const char* Name)
{
    if (!jv.isObject())
        return false;

    if (!jv.isMember(Name))
        return false;

    if (!jv[Name].isArray())
        return false;

    return true;
}

bool CJSONHelper::HasObject(const Json::Value& jv, const char* Name)
{
    if (!jv.isObject())
        return false;

    if (!jv.isMember(Name))
        return false;

    if (!jv[Name].isObject())
        return false;

    return true;
}

std::string CJSONHelper::GetString(const Json::Value& jv, const char* Name, const char* Default)
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

bool CJSONHelper::GetBool(const Json::Value& jv, const char* Name, bool Default)
{
    if (!jv.isObject())
        return Default;

    if (!jv.isMember(Name))
        return Default;

    return jv[Name].asBool();
}

int CJSONHelper::GetInt(const Json::Value& jv, const char* Name, int Default)
{
    if (!jv.isObject())
        return Default;

    if (!jv.isMember(Name))
        return Default;

    if (!jv[Name].isInt())
        return Default;

    return jv[Name].asInt();
}