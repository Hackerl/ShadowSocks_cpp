//
// Created by patteliu on 2018/10/24.
//

#include "CommonProxy.h"
#include <Socket/LibSocketExport.h>

CommonProxy::CommonProxy()
{
    m_HasInit = false;
}

bool CommonProxy::OnPipeIn(const void *Buffer, size_t Length)
{
    if (!m_HasInit)
    {
        m_HasInit = true;

        //if (Length < sizeof(int))

    }

    return m_PipeNode->PipeOut(Buffer, Length);
}
