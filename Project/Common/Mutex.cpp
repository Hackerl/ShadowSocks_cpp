//
// Created by patteliu on 2018/10/12.
//

#include "Common/Mutex.h"

Mutex::Mutex()
{
    pthread_mutex_init(&m_lock, nullptr);
}

Mutex::~Mutex()
{
    pthread_mutex_destroy(&m_lock);
}

void Mutex::Lock()
{
    pthread_mutex_lock(&m_lock);
}

void Mutex::UnLock()
{
    pthread_mutex_unlock(&m_lock);
}
