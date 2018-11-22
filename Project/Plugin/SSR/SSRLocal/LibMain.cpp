//
// Created by hackerl on 10/21/18.
//
#include <iostream>
#include "SSRLocal.h"

int main()
{
    CSSRLocal SSRLocal;
    u_char Data[10] = {0x05, 0x01, 0x00, 0x01, 0x0e, 0xd7, 0xb1, 0x27, 0x00, 0x50};

    SSRLocal.OnUpStream(Data, 10);

    std::cout << "Hello, World!" << std::endl;
    return 0;
}