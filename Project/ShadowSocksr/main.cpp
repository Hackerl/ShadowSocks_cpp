#include "CShadowSocks.h"
#include <fstream>

int main()
{
    std::ifstream InFile("Config.json");

    if (!InFile.is_open())
    {
        LOG(ERROR) << "Cant Not Open Config File";
        return 0;
    }

    Json::Value jv;
    Json::Reader jr;

    if (!jr.parse(InFile, jv))
    {
        LOG(ERROR) << "Cant Not Parse Config File";
        return 0;
    }

    CShadowSocks ShadowSocks;

    if (!ShadowSocks.SetConfig(jv))
    {
        LOG(ERROR) << "ShadowSocks Cant Not Set Config";
        return 0;
    }

    ShadowSocks.Run();

    return 0;
}