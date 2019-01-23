//
// Created by hackerl on 11/4/18.
//

#ifndef SSRPLUGIN_RANDOM_H
#define SSRPLUGIN_RANDOM_H

#include <random>
#include <Common/Singleton.h>

class CRandom
{
#define g_Random SINGLETON_(CRandom)
public:
    unsigned int Next()
    {
        return m_RandomDev();
    }

private:
    std::random_device m_RandomDev;
};

#endif //SSRPLUGIN_RANDOM_H
