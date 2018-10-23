//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_TCPRELAY_H
#define SHADOWSOCKSR_CPP_TCPRELAY_H

#include "CommonSocketRelay.h"

class CSocketRelay : public CCommonSocketRelay
{
public:
    CSocketRelay();
    ~CSocketRelay();

public:
    void SetPlugin(IPlugin * Plugin);

public:
    bool OnDataIn(const void *Buffer, size_t Length) override;
    bool OnPipeIn(const void *Buffer, size_t Length) override;

private:
    IPlugin * m_Plugin;
};
#endif //SHADOWSOCKSR_CPP_TCPRELAY_H
