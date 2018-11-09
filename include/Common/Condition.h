//
// Created by patteliu on 2018/11/9.
//

#ifndef SHADOWSOCKSR_CPP_CONDITION_H
#define SHADOWSOCKSR_CPP_CONDITION_H

#include "Mutex.h"

class Condition
{
public:
    Condition() : m_Cond()
    {
        pthread_cond_init(&m_Cond, nullptr);
    }

    ~Condition()
    {
        pthread_cond_destroy(&m_Cond);
    }

public:
    void Wait()
    {
        m_Mutex.Lock();

        pthread_cond_wait(&m_Cond, m_Mutex.Get());

        m_Mutex.UnLock();
    }

    void Notify()
    {
        pthread_cond_signal(&m_Cond);
    }

    void NotifyAll()
    {
        pthread_cond_broadcast(&m_Cond);
    }

private:
    pthread_cond_t m_Cond;
    Mutex m_Mutex;
};

#endif //SHADOWSOCKSR_CPP_CONDITION_H
