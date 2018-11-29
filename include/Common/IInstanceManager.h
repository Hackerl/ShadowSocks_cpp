//
// Created by patteliu on 2018/10/19.
//

#ifndef SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H
#define SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H

#include "Interface.h"

class IInstanceManager : public Interface
{
public:
    virtual void AddRef() = 0;
    virtual void Release() = 0;
};

template <class T>
inline void AddRef(T * Instance)
{
    auto InstanceMgr = dynamic_cast<IInstanceManager *>(Instance);

    if (InstanceMgr != nullptr)
        InstanceMgr->AddRef();
}

template <class T>
inline void Release(T * Instance)
{
    auto InstanceMgr = dynamic_cast<IInstanceManager *>(Instance);

    if (InstanceMgr != nullptr)
        InstanceMgr->Release();
}
#endif //SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H
