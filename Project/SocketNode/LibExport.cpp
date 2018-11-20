//
// Created by patteliu on 2018/10/26.
//

#include "LocalSocketNode.h"
#include "RemoteSocketNode.h"
#include <Common/InstanceManager.h>
#include <SocketNode/LibSocketNodeExport.h>

extern "C" ISocketNode * NewLocalSocketNode()
{
    return new InstanceManager<CLocalSocketNode>;
}

extern "C" ISocketNode * NewRemoteSocketNode()
{
    return new InstanceManager<CRemoteSocketNode>;
}