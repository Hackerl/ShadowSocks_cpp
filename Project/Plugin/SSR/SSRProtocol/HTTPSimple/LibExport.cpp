//
// Created by patteliu on 2018/11/23.
//

#include "HTTPSimple.h"
#include <Protocol/LibProtocolExport.h>

extern "C" IProtocol * NewProtocol(const char * ProtocolName)
{
    if (std::string(ProtocolName) != "http_simple")
        return nullptr;

    return new CHTTPSimple;
}