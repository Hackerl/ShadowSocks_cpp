//
// Created by patteliu on 2018/10/26.
//

#include "SocketConnector.h"

CSocketConnector::CSocketConnector() : m_Config()
{
    m_Config.ProxyType = NOProxyType;
}

bool CSocketConnector::SetConfig(Json::Value &Config)
{
    //TODO parse config
    return true;
}

bool CSocketConnector::OnNodeInit(void *arg)
{
    //TODO parse target ip and port, connect with proxy config, init remote socket node
    return true;
}
