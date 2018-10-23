//
// Created by patteliu on 2018/10/23.
//
#include "PortTunnel.h"
#include "Plugin/LibPluginExport.h"

extern "C" IPlugin * NewPortTunnel()
{
    return new CPortTunnel;
}
