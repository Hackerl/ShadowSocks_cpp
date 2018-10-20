//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_TCPRELAY_H
#define SHADOWSOCKSR_CPP_TCPRELAY_H

#include "Socket/ISocket.h"
#include "Pipe/IPipe.h"
#include "Pipe/Pipe.h"
#include "Pipe/PipeDelay.h"

class CTCPRelay :public CPipe, public ISocketEventCallback
{
public:
    bool PipeOut(const char * Buffer, size_t Length) override;
    void OnPipeClose() override;

public:
    void OnRead(int fd, short Event) override ;
    void OnWrite(int fd, short Event) override;
    void OnClose(int fd, short Event) override;
};


#endif //SHADOWSOCKSR_CPP_TCPRELAY_H
