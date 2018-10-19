//
// Created by patteliu on 2018/10/19.
//

#ifndef SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H
#define SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H

class IInstanceManager
{
public:
    virtual void AddRef() = 0;
    virtual void Release() = 0;
};

#endif //SHADOWSOCKSR_CPP_IINSTANCEMANAGER_H
