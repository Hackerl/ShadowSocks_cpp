#include "CShadowSocks.h"
#include <fstream>
#include <climits>
#include <libgen.h>

void SetWorkPath()
{
    char ExePath[PATH_MAX] = {};
    readlink("/proc/self/exe", ExePath, PATH_MAX);
    chdir(dirname(ExePath));
}

int main(int argc, char ** argv)
{
    SetWorkPath();

    FLAGS_alsologtostderr = true;
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

        InFile.close();
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