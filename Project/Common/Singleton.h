//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_SINGLETON_H
#define SHADOWSOCKSR_CPP_SINGLETON_H

#include "Mutex.h"

template<typename T>
struct Singleton
{
    static T* Instance();
    static T* InstanceNoLock();
};

#define SINGLETON_(class) ((class*)Singleton<class>::Instance())
#define SINGLETON_NOLOCK_(class) ((class*)Singleton<class>::InstanceNoLock())

#endif //SHADOWSOCKSR_CPP_SINGLETON_H
