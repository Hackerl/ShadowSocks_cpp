#include "ShadowSocks.h"
#include <fstream>
#include <climits>
#include <libgen.h>
#include <Common/CmdLine.h>

void SetWorkPath()
{
    char ExePath[PATH_MAX] = {};
    readlink("/proc/self/exe", ExePath, PATH_MAX);
    chdir(dirname(ExePath));
}

Json::Value ReadConfig(const char * Path)
{
    Json::Value Config;

    std::ifstream ConfigFile(Path);

    if (!ConfigFile.is_open())
    {
        LOG(ERROR) << "Cant Not Open Config File: " << Path;
        return Config;
    }

    Json::Reader jr;

    if (!jr.parse(ConfigFile, Config))
    {
        LOG(ERROR) << "Cant Not Parse Config File";
    }

    return Config;
}

int main(int argc, char ** argv)
{
    FLAGS_alsologtostderr = true;
    google::InitGoogleLogging(argv[0]);

    SetWorkPath();

    cmdline::parser CmdParser;

    CmdParser.add<std::string>("config", 'c', "Config File Path", false, "Config.json");
    CmdParser.parse_check(argc, argv);

    std::string Path = CmdParser.get<std::string>("config");
    Json::Value Config = ReadConfig(Path.c_str());

    CShadowSocks ShadowSocks;

    if (!ShadowSocks.SetConfig(Config))
    {
        LOG(ERROR) << "ShadowSocks Set Config Failed";
        return 0;
    }

    ShadowSocks.Run();

    return 0;
}