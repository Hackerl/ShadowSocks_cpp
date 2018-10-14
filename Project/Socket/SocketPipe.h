//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_SOCKETPIPE_H
#define SHADOWSOCKSR_CPP_SOCKETPIPE_H

#include "Socket/ISocketPipe.h"

class CSocketPipe : public ISocketPipe
{
public:
    CSocketPipe();
    CSocketPipe(ISocketPipe * PipePort);

public:
    void SetPipePort(ISocketPipe * PipePort);

public:
    bool PipeIn(const char * Buffer, size_t Length) override;
    bool PipeOut(const char * Buffer, size_t Length) override;
    void PipeClose() override;
    void OnPipeClose() override;

private:
    bool m_PipeClosed;
    ISocketPipe * m_PipePort;
};


#endif //SHADOWSOCKSR_CPP_SOCKETPIPE_H
