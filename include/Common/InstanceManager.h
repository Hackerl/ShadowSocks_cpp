//
// Created by patteliu on 2018/10/19.
//

#ifndef SHADOWSOCKSR_CPP_INSTANCEMANAGER_H
#define SHADOWSOCKSR_CPP_INSTANCEMANAGER_H

#include "IInstanceManager.h"
#include "Mutex.h"
#include <iostream>

template <class T>
class InstanceManager : public IInstanceManager , public T
{
public:
    InstanceManager()
    {
        m_Ref = 0;
    }

public:
    void AddRef() override
    {
        AutoMutex _0_(m_Mutex);
        m_Ref ++;
        std::cout << "addref" << m_Ref << std::endl;
    }

    void Release() override
    {
        AutoMutex _0_(m_Mutex);

        std::cout << "release" << m_Ref - 1 << std::endl;

        if (-- m_Ref == 0)
        {
            std::cout << "Delete" << std::endl;
            delete this;
        }
    }

private:
    Mutex m_Mutex;
    int m_Ref;
};


#endif //SHADOWSOCKSR_CPP_INSTANCEMANAGER_H
