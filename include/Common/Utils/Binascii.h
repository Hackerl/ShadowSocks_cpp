//
// Created by hackerl on 10/28/18.
//

#ifndef SSRPLUGIN_BINASCII_H
#define SSRPLUGIN_BINASCII_H

#include <vector>
#include <sstream>

constexpr char HexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7',
                           '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
class CBinascii
{
public:
    static std::string Hexlify(const u_char * Buffer, size_t Length)
    {
        std::string s(Length * 2, ' ');

        for (int i = 0; i < Length; ++i)
        {
            s[2 * i]     = HexMap[(Buffer[i] & 0xF0) >> 4];
            s[2 * i + 1] = HexMap[Buffer[i] & 0x0F];
        }

        return s;
    }

    static std::vector<u_char> UnHexlify(std::string HexString)
    {
        std::vector<u_char> Buffer;

        for(size_t i = 0; i < HexString.size(); i += 2)
        {
            int Number;

            std::istringstream Stream(HexString.substr(i, 2));
            Stream >> std::hex >> Number;

            Buffer.push_back(u_char(Number & 0xff));
        }

        return Buffer;
    }
};

#endif //SSRPLUGIN_BINASCII_H
