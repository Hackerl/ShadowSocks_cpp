//
// Created by hackerl on 10/28/18.
//

#include "HTTPSimple.h"
#include "TLSTicketAuth.h"

int main()
{
    CTLSTicketAuth _0_;

    u_char Data[4] = {0x00, 0x11, 0x22, 0x33};

    std::vector<u_char> EncodeStream = _0_.Encode(Data, 4);

    return 0;
}