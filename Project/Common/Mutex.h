//
// Created by patteliu on 2018/10/12.
//

#ifndef SHADOWSOCKSR_CPP_MUTEX_H
#define SHADOWSOCKSR_CPP_MUTEX_H

#include <pthread.h>

class Mutex {
public:
    Mutex();
    ~Mutex();
    void Lock();
    void UnLock();

private:
    pthread_mutex_t m_lock;
};
#endif //SHADOWSOCKSR_CPP_MUTEX_H
