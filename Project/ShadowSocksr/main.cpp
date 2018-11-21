#include "ShadowSocks.h"
#include <fstream>

int main(int argc, char ** argv)
{
    FLAGS_alsologtostderr = 1;
    google::InitGoogleLogging(argv[0]);

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

    InFile.close();

    CShadowSocks ShadowSocks;

    if (!ShadowSocks.SetConfig(jv))
    {
        LOG(ERROR) << "ShadowSocks Cant Not Set Config";
        return 0;
    }

    ShadowSocks.Run();

    return 0;
}