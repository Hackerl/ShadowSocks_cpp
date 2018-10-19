//
// Created by patteliu on 2018/10/19.
//

#include <Common/InstanceManager.h>

#include "Common/InstanceManager.h"

InstanceManager::InstanceManager()
{
    m_Ref = 0;
}

void InstanceManager::AddRef()
{
    AutoMutex _0_(m_Mutex);
    m_Ref ++;
}

void InstanceManager::Release()
{
    AutoMutex _0_(m_Mutex);

    if (-- m_Ref == 0)
        delete this;
}
