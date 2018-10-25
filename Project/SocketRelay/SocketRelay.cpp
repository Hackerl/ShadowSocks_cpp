//
// Created by hackerl on 10/14/18.
//

#include "SocketRelay.h"

CSocketRelay::CSocketRelay()
{
    m_Plugin = nullptr;
}

CSocketRelay::~CSocketRelay()
{
    if (m_Plugin != nullptr)
    {
        delete m_Plugin;
        m_Plugin = nullptr;
    }
}

void CSocketRelay::SetPlugin(IPlugin * Plugin)
{
    m_Plugin = Plugin;
    //Plugin->SetPipe(this);
}

bool CSocketRelay::OnDataIn(const void *Buffer, size_t Length)
{
    if (m_Plugin == nullptr)
        return false;

    //return m_Plugin->OnDataIn(Buffer, Length);
}

bool CSocketRelay::OnPipeIn(const void *Buffer, size_t Length)
{
    if (m_Plugin == nullptr)
        return false;

    //return m_Plugin->OnPipeIn(Buffer, Length);
}
