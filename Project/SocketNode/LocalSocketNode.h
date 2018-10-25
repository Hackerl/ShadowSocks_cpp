//
// Created by patteliu on 2018/10/25.
//

#ifndef SHADOWSOCKSR_CPP_LOCALSOCKETNODE_H
#define SHADOWSOCKSR_CPP_LOCALSOCKETNODE_H

#include "SocketNode.h"

class CLocalSocketNode : public CSocketNode
{
public:
    bool DataIn(const void *Buffer, size_t Length) override
    {
        return UpStream(Buffer, Length);
    }

    bool OnDownStream(const void *Buffer, size_t Length) override
    {
        return DataOut(Buffer, Length);
    }
};


#endif //SHADOWSOCKSR_CPP_LOCALSOCKETNODE_H
