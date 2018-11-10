//
// Created by patteliu on 2018/11/9.
//

#ifndef SHADOWSOCKSR_CPP_CONDITION_H
#define SHADOWSOCKSR_CPP_CONDITION_H

#include "Mutex.h"

class Condition
{
public:
    Condition() : m_Cond(), m_Signal(false)
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

        while (!m_Signal)
            pthread_cond_wait(&m_Cond, m_Mutex.Get());

        m_Signal = false;

        m_Mutex.UnLock();
    }

    void Notify()
    {
        m_Mutex.Lock();

        m_Signal = true;
        pthread_cond_signal(&m_Cond);

        m_Mutex.UnLock();
    }

    void NotifyAll()
    {
        m_Mutex.Lock();

        m_Signal = true;
        pthread_cond_broadcast(&m_Cond);

        m_Mutex.UnLock();
    }

private:
    bool m_Signal;
    pthread_cond_t m_Cond;
    Mutex m_Mutex;
};

#endif //SHADOWSOCKSR_CPP_CONDITION_H
