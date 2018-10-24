//
// Created by patteliu on 2018/10/24.
//

#include "Common/InstanceManager.h"
#include "SocketRelay.h"
#include "SocketRelay/LibSocketRelayExport.h"
#include "Pipe/Pipe.h"

extern "C" ISocketRelay * NewSocketRelay()
{
    return new InstanceManager<CSocketRelay>;
}

extern "C" ISocketRelay * NewCommonSocketRelay()
{
    return new InstanceManager<CCommonSocketRelay>;
}