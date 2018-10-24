//
// Created by patteliu on 2018/10/24.
//

#include "Socks5.h"
#include "Plugin/LibPluginExport.h"

extern "C" IPlugin * NewSocks5Proxy()
{
    return new CSocks5Proxy;
}
