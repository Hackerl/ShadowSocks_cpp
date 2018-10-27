//
// Created by patteliu on 2018/10/23.
//

#ifndef SHADOWSOCKSR_CPP_LIBPLUGINEXPORT_H
#define SHADOWSOCKSR_CPP_LIBPLUGINEXPORT_H

#include "IPlugin.h"

extern "C" IPlugin * NewPortTunnel();
extern "C" IPlugin * NewSocks5Proxy();
extern "C" IPlugin * NewSocksServer();
extern "C" IPlugin * NewSocketConnector();

#endif //SHADOWSOCKSR_CPP_LIBPLUGINEXPORT_H
