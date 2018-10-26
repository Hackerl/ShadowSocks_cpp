//
// Created by patteliu on 2018/10/26.
//

#ifndef SHADOWSOCKSR_CPP_LIBSOCKETNODEEXPORT_H
#define SHADOWSOCKSR_CPP_LIBSOCKETNODEEXPORT_H

#include "ISocketNode.h"

extern "C" ISocketNode * NewLocalSocketNode();
extern "C" ISocketNode * NewRemoteSocketNode();

#endif //SHADOWSOCKSR_CPP_LIBSOCKETNODEEXPORT_H
