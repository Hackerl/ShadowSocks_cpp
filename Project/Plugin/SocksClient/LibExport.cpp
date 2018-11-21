//
// Created by patteliu on 2018/10/24.
//

#include "SocksClient.h"
#include <Common/InstanceManager.h>
#include <Plugin/LibPluginExport.h>

extern "C" INode * NewPlugin()
{
    return new InstanceManager<CSocks5Proxy>;
}
