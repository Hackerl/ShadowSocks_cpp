//
// Created by hackerl on 10/21/18.
//

#ifndef SSRPLUGIN_XORSHIFT128PLUS_H
#define SSRPLUGIN_XORSHIFT128PLUS_H

#include <cstdint>
#include <cstring>

class XorShift128Plus
{
public:
    XorShift128Plus()
    {
        v0 = 0;
        v1 = 0;
    }

public:
    uint64_t Next()
    {
        uint64_t x = v0;
        uint64_t y = v1;
        v0 = y;
        x ^= x << 23;
        x ^= (y ^ (x >> 17) ^ (y >> 26));
        v1 = x;
        return x + y;
    }

    void InitFromBin(uint8_t * Bin, int Size)
    {
        uint8_t FillBin[16] = { 0 };
        memcpy(FillBin, Bin, Size < 16 ? Size : 16);

        v0 = * (uint64_t *) FillBin;
        v1 = * ((uint64_t *) FillBin + 1);
    }

    void InitFromBinLen(uint8_t * Bin, int Size, int Len)
    {
        uint8_t FillBin[16] = { 0 };
        memcpy(FillBin, Bin, Size < 16 ? Size : 16);

        FillBin[0] = Len & 0xff;
        FillBin[1] = (Len >> 8) & 0xff;

        v0 = * (uint64_t *) FillBin;
        v1 = * ((uint64_t *) FillBin + 1);

        for (int i = 0; i < 4; i ++)
            Next();
    }

private:
    uint64_t v0;
    uint64_t v1;
};

#endif //SSRPLUGIN_XORSHIFT128PLUS_H
