//
// Created by patteliu on 2018/11/23.
//

#include "TLSTicketAuth.h"
#include <Protocol/LibProtocolExport.h>

extern "C" IProtocol * NewProtocol(const char * ProtocolName)
{
    if (std::string(ProtocolName) != "tls1.2_ticket_auth")
        return nullptr;

    return new CTLSTicketAuth;
}