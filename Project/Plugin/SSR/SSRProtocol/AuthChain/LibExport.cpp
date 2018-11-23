//
// Created by patteliu on 2018/11/23.
//

#include "AuthChainLocal.h"
#include <Protocol/LibProtocolExport.h>

extern "C" IProtocol * NewProtocol(const char * ProtocolName)
{
    if (std::string(ProtocolName) != "auth_chain_a")
        return nullptr;

    return new CAuthChainLocal;
}