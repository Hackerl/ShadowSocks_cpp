//
// Created by patteliu on 2018/10/24.
//

#ifndef SHADOWSOCKSR_CPP_LIBSOCKETRELAYEXPORT_H
#define SHADOWSOCKSR_CPP_LIBSOCKETRELAYEXPORT_H

#include "ISocketRelay.h"

extern "C" ISocketRelay * NewSocketRelay();
extern "C" ISocketRelay * NewCommonSocketRelay();

#endif //SHADOWSOCKSR_CPP_LIBSOCKETRELAYEXPORT_H
