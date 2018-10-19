//
// Created by patteliu on 2018/10/19.
//

#ifndef SHADOWSOCKSR_CPP_INSTANCEMANAGER_H
#define SHADOWSOCKSR_CPP_INSTANCEMANAGER_H

#include "IInstanceManager.h"
#include "Mutex.h"

class InstanceManager : public IInstanceManager
{
public:
    InstanceManager();

public:
    void AddRef() override;
    void Release() override;

private:
    Mutex m_Mutex;
    int m_Ref;
};


#endif //SHADOWSOCKSR_CPP_INSTANCEMANAGER_H
