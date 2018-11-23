//
// Created by patteliu on 2018/11/23.
//

#ifndef SHADOWSOCKSR_CPP_LIBPROTOCOLEXPORT_H
#define SHADOWSOCKSR_CPP_LIBPROTOCOLEXPORT_H

#include "IProtocol.h"

extern "C" IProtocol * NewProtocol(const char * ProtocolName);

#endif //SHADOWSOCKSR_CPP_LIBPROTOCOLEXPORT_H
