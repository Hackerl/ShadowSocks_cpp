//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_ISOCKETPIPE_H
#define SHADOWSOCKSR_CPP_ISOCKETPIPE_H

#include <iostream>

class ISocketPipe
{
public:
    virtual bool PipeIn(const char * Buffer, size_t Length) = 0;
    virtual bool PipeOut(const char * Buffer, size_t Length) = 0;
    virtual void PipeClose() = 0;
    virtual void OnPipeClose() = 0;
};

#endif //SHADOWSOCKSR_CPP_ISOCKETPIPE_H
