//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKETRELAY_H
#define SHADOWSOCKSR_CPP_ISOCKETRELAY_H

#include "Pipe/IPipe.h"
#include "Pipe/Pipe.h"
#include "Socket/ISocket.h"

class ISocketRelay : public CPipe, public ISocketClientCallback
{

};


#endif //SHADOWSOCKSR_CPP_ISOCKETRELAY_H
