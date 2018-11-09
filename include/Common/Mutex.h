//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_MUTEX_H
#define SHADOWSOCKSR_CPP_MUTEX_H

#include <pthread.h>

class Mutex
{
public:
    Mutex() : m_lock()
    {
        pthread_mutex_init(&m_lock, nullptr);
    }

    ~Mutex()
    {
        pthread_mutex_destroy(&m_lock);
    }

public:
    pthread_mutex_t * Get()
    {
        return &m_lock;
    }

public:
    void Lock()
    {
        pthread_mutex_lock(&m_lock);
    }

    void UnLock()
    {
        pthread_mutex_unlock(&m_lock);
    }

private:
    pthread_mutex_t m_lock;
};

template<typename T>
class AutoMutex_
{
public:
    explicit AutoMutex_(T& Lock) : m_Lock(Lock)
    {
        m_Lock.Lock();
    }

    ~AutoMutex_()
    {
        m_Lock.UnLock();
    }

private:
    T&	m_Lock;
};
//******************************************************************************
typedef AutoMutex_<Mutex> AutoMutex;
#endif //SHADOWSOCKSR_CPP_MUTEX_H
