//
// Created by patteliu on 2018/10/25.
//

#include "SocksServer.h"
#include <Common/InstanceManager.h>
#include <Plugin/LibPluginExport.h>

extern "C" IPlugin * NewPlugin()
{
    return new InstanceManager<CProxyServer>;
}