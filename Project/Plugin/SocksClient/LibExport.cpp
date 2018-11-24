//
// Created by patteliu on 2018/10/24.
//

#include "SocksClient.h"
#include <Plugin/LibPluginExport.h>

extern "C" IPlugin * NewPlugin()
{
    return new CSocks5Proxy;
}
