//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_SINGLETON_H
#define SHADOWSOCKSR_CPP_SINGLETON_H

#include "Mutex.h"

template<typename T>
struct Singleton
{
    static T* Instance()
    {
        static Mutex MutexLock;

        MutexLock.Lock();

        static T __instance__;

        MutexLock.UnLock();

        return &__instance__;
    }

    static T* InstanceNoLock()
    {
        static T __instance__;
        return &__instance__;
    }
};

#define SINGLETON_(class, ...) ((class*)Singleton<class>::Instance())
#define SINGLETON_NOLOCK_(class, ...) ((class*)Singleton<class>::InstanceNoLock())

#endif //SHADOWSOCKSR_CPP_SINGLETON_H
