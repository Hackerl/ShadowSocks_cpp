//
// Created by patteliu on 2018/10/25.
//

#include "CommonProxy.h"
#include "Common/InstanceManager.h"
#include "Plugin/LibPluginExport.h"

extern "C" IPlugin * NewCommonProxy()
{
    return new InstanceManager<CommonProxy>;
}