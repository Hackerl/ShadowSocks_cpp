//
// Created by patteliu on 2018/10/26.
//

#include "LocalSocketNode.h"
#include "RemoteSocketNode.h"
#include <SocketNode/LibSocketNodeExport.h>

extern "C" ISocketNode * NewLocalSocketNode()
{
    return new CLocalSocketNode;
}

extern "C" ISocketNode * NewRemoteSocketNode()
{
    return new CRemoteSocketNode;
}