//
// Created by patteliu on 2018/10/24.
//

#include "Socks5.h"
#include "Common/InstanceManager.h"
#include "Plugin/LibPluginExport.h"

extern "C" IPlugin * NewSocks5Proxy()
{
    return new InstanceManager<CSocks5Proxy>;
}
