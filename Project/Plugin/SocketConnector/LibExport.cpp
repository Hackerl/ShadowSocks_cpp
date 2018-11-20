//
// Created by hackerl on 10/27/18.
//

#include "SocketConnector.h"
#include <Plugin/LibPluginExport.h>
#include <Common/InstanceManager.h>

extern "C" IPlugin * NewPlugin()
{
    return new InstanceManager<CSocketConnector>;
}