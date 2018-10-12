//
// Created by patteliu on 2018/10/12.
//

#include "Singleton.h"

template<typename T>
T *Singleton<T>::Instance()
{
    static Mutex MutexLock;

    MutexLock.Lock();

    static T __instance__;

    MutexLock.UnLock();

    return &__instance__;
}

template<typename T>
T *Singleton<T>::InstanceNoLock()
{
    static T __instance__;
    return &__instance__;
}
