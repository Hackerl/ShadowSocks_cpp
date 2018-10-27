//
// Created by hackerl on 10/27/18.
//

#include "Plugin/LibPluginExport.h"
#include "Common/InstanceManager.h"
#include "SocketConnector.h"

extern "C" IPlugin * NewSocketConnector()
{
    return new InstanceManager<CSocketConnector>;
}