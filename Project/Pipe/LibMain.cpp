//
// Created by hackerl on 10/14/18.
//
#include "Pipe/Pipe.h"
#include "Common/InstanceManager.h"

class CTestPipe : public CPipe
{
public:
    CTestPipe()
    {
        m_ID = -1;
    }

    ~CTestPipe() override
    {
        std::cout << m_ID << "Delete Pipe" << std::endl;
    }

public:
    bool PipeOut(const char * Buffer, size_t Length) override
    {
        std::cout << m_ID << "Pipe Out" << std::endl;
    };

    void OnPipeClose() override
    {
        std::cout << m_ID << "Pipe Close" << std::endl;
    };

public:
    int m_ID;
};

void TestPipe()
{
    auto Pipe1 = new InstanceManager<CTestPipe>;
    AddRef(Pipe1);

    Pipe1->m_ID = 1;

    auto Pipe2 = new InstanceManager<CTestPipe>;
    AddRef(Pipe2);

    Pipe2->m_ID = 2;

    Pipe2->PipeConnect(Pipe1);
    Pipe1->PipeConnect(Pipe2);

    std::string msg = "hello";

    Pipe2->PipeIn(msg.c_str(), msg.length());
    Pipe2->PipeClose();

    Release(Pipe1);
    Release(Pipe2);
}

int main()
{
    TestPipe();
    return 0;
}