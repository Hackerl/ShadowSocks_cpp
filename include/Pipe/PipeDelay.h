//
// Created by hackerl on 10/14/18.
//

#ifndef SHADOWSOCKSR_CPP_PIPEDELAY_H
#define SHADOWSOCKSR_CPP_PIPEDELAY_H

#include "Pipe.h"

template <class T>
class CPipeDelay : public CPipe
{
public:
    CPipeDelay()
    {
        m_arg = nullptr;
    }

public:
    bool PipeOut(const char * Buffer, size_t Length) override
    {
        if (m_PipePort == nullptr)
            return false;

        auto * Instance = new T;

        do
        {
            auto DelayInstance = dynamic_cast<IDelayInstance *>(Instance);

            if (DelayInstance == nullptr)
                break;

            if(!DelayInstance->InitDelay(m_arg))
                break;

            auto Pipe = dynamic_cast<IPipe *>(Instance);

            if (Pipe == nullptr)
                break;

            Pipe->PipeConnect(m_PipePort);

            m_PipePort->PipeConnect(Pipe);

            m_PipePort = nullptr;

            Pipe->PipeOut(Buffer, Length);

            Destory();

            return true;

        } while (false);

        delete(Instance);

        Destory();

        return false;
    }

    void OnPipeClose() override
    {
        Destory();
    }

public:
    void SetArg(void * arg)
    {
        m_arg = arg;
    }

private:
    void Destory()
    {
        if (m_PipePort != nullptr)
            m_PipePort->PipeClose();

        delete this;
    }

private:
    void * m_arg;
};

#endif //SHADOWSOCKSR_CPP_PIPEDELAY_H
